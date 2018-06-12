#include "native.h"

#include "../native/stdlib.h"

static void make_native(Vm *vm, const char *name, void *fn_ptr, uint8_t num_params, bool pass_vm) {
    ObjNativeFunc *println = new_native_func(vm, fn_ptr, num_params, pass_vm);
    Value value = create_object((Object *) println);

    HTItemKey key;
    key.key_c_string = name;
    ht_put(&vm->compiler.natives, key, value);
}

void declare_natives(Vm *vm) {
    make_native(vm, "println", println, 1, false);
    make_native(vm, "print", print, 1, false);
    make_native(vm, "exit", exit_vm, 1, false);
    make_native(vm, "str", str, 1, true);
}
