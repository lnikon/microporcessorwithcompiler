#include <vpi_user.h>

static int increment(char *user_data) {
    vpiHandle sys_tf_ref, args_iter, arg_h;
    struct t_vpi_value arg_val;
    int value;

    // Obtain a handle to the argument list
    sys_tf_ref = vpi_handle(vpiSysTfCall, NULL);
    args_iter = vpi_iterate(vpiArgument, sys_tf_ref);

    // Grab the value of the first argument
    arg_h = vpi_scan(args_iter);
    arg_val.format = vpiIntVal;
    vpi_get_value(arg_h, &arg_val);
    value = arg_val.value.integer;
    vpi_printf("VPI Routine Received %d\n", value);

    // Increment the value and put it back as first argument
    arg_val.value.integer = value + 1;
    vpi_put_value(arg_h, &arg_val, NULL, vpiNoDelay);

    // Cleanup and return
    vpi_free_object(args_iter);

    return 0;
}

// Register the increment system task
void register_increment() {
    s_vpi_systf_data data = {vpiSysTask,
                            0,
                            "$increment",
                            increment,
                            0,
                            0,
                            0};
    vpi_register_systf(&data);
}

void (*vlog_startup_routines[])() = {
    register_increment,
    0
};
                                
