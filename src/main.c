#include <stdio.h>
#include <string.h>

#include "chunk.h"
#include "value.h"
#include "debug.h"
#include "vm.h"
#include "opcode.h"

int main(void) {
    Chunk chunk;
    init_chunk(&chunk);

    /*
    uint32_t constant = add_constant(&chunk, 4.2);
    write_chunk(&chunk, OP_LDC);
    write_chunk(&chunk, constant);
    write_chunk(&chunk, OP_NEGATE);

    constant = add_constant(&chunk, 7.0);
    write_chunk(&chunk, OP_LDC);
    write_chunk(&chunk, constant);

    write_chunk(&chunk, OP_ADD);

    constant = add_constant(&chunk, 2);
    write_chunk(&chunk, OP_LDC);
    write_chunk(&chunk, constant);

    write_chunk(&chunk, OP_MUL);

    write_chunk(&chunk, OP_STORE);
    write_chunk(&chunk, (uint8_t)0);

    write_chunk(&chunk, OP_LOAD);
    write_chunk(&chunk, (uint8_t)0);

    write_chunk(&chunk, OP_DUP);
    write_chunk(&chunk, OP_DIV);


    uint32_t constant = add_constant(&chunk, 10);
    write_chunk(&chunk, OP_LDC);
    write_chunk(&chunk, (uint8_t) constant);
    
    constant = add_constant(&chunk, (Value)0); // counter
    write_chunk(&chunk, OP_LDC);
    write_chunk(&chunk, (uint8_t) constant);
    write_chunk(&chunk, OP_STORE);
    write_chunk(&chunk, (uint8_t)1); // save counter
    
    constant = add_constant(&chunk, (Value)10); // while < 10
    write_chunk(&chunk, OP_LDC);
    write_chunk(&chunk, (uint8_t) constant);
    write_chunk(&chunk, OP_STORE);
    write_chunk(&chunk, (uint8_t)2);
    
    write_chunk(&chunk, OP_DUP);
    write_chunk(&chunk, OP_PRINT);
    
    write_chunk(&chunk, OP_DEC);
    write_chunk(&chunk, (uint8_t)1); // number - 1
    
    write_chunk(&chunk, OP_LOAD);
    write_chunk(&chunk, (uint8_t)1); // load counter
    write_chunk(&chunk, OP_INC);
    write_chunk(&chunk, (uint8_t)1); // increment counter
    write_chunk(&chunk, OP_DUP);
    write_chunk(&chunk, OP_STORE);
    write_chunk(&chunk, (uint8_t)1);
    
    write_chunk(&chunk, OP_LOAD);
    write_chunk(&chunk, (uint8_t)2); // load 10
    
    write_chunk(&chunk, OP_JLT);
    write_chunk(&chunk, (uint8_t)10); // jump to dup before print
    
    write_chunk(&chunk, OP_RETURN);
    */

    uint8_t number = (uint8_t) add_constant(&chunk, 10);

    for(int i = 0; i < 256; ++i) {
        add_constant(&chunk, 0);
    }

    uint16_t condition = (uint16_t) add_constant(&chunk, 10);

    uint8_t first_index = (uint8_t) (condition >> 8);
    uint8_t second_index = (uint8_t) (condition & 0xFF);

    uint8_t code[] = {
            OP_LDC, number,       // load number to print
            OP_LDC_0,             // i
            OP_STORE, 0,
            OP_LDC_W, first_index, second_index,    // 10
            OP_STORE, 1,
            OP_DUP, OP_PRINT,     // print number
            OP_DEC, 1,            // number--
            OP_LOAD, 0,
            OP_INC, 1,            // i++
            OP_DUP,
            OP_STORE, 0,
            OP_LOAD, 1,
            OP_JLT, 10,            // if i < 10 jump
            OP_RETURN
    };

    size_t size = sizeof(code);
    uint8_t  *code_heap = malloc(size);
    memcpy(code_heap, code, size);

    init_chunk_direct(&chunk, code_heap, size);

    Vm vm;
    init_vm(&vm);

    interpret(&vm, &chunk);

    // disassemble_chunk(&chunk, "10 - 1 Zähler");

    free_vm(&vm);
    free_chunk(&chunk);
    return 0;
}