#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "codegen.h"

#define MAXBUFF 1024

FILE* assembly;


void goto_end_of_data_sec(char* to_add)
{
    
    // First reset the file pointer to start
    fseek(assembly, 0, SEEK_SET);

    // First we read the asm file
    char buff[MAXBUFF];
    int line_no = 1, count=1;

    // Until eof
    while(fscanf(assembly, "%s", buff) != 0)
    {
        // Note the line number of where .text starts
        if(!strcmp(buff, "section       .text"))
            break;
        
        line_no++;
    }

    // Close assembly file
    fclose(assembly);
    // To account for the two \n's before section .text
    line_no -= 2;

    // Open temp file to add the required line
    FILE* fp = fopen("temp.asm", "w");
    assembly = fopen("assembly_try.asm","rw");


    if(!fp)
    {
        printf("[-] Error opening file!...Exiting\n");
        exit(1);
    }

    else
    {
        // Until eof
        while(fscanf(assembly, "%s", buff) != 0)
        {
            // Write all except the marked line
            if(count != line_no)
                fprintf(fp,"%s",buff);
            
            // Add the required line
            else
            {
                fprintf(fp,"%s",to_add);
                count++;
            }
            
        }
    }

    fclose(fp);
    fclose(assembly);

    // Replace assembly_try.asm by temp.asm by renaming
    remove("assembly_try.asm");
    rename("temp.asm","assembly_try.asm");
}

void macros_starter(){
    /* This function defines the required macros in the assembly file
    */

    fprintf(assembly, "\t\t;macro to store all registers\n");
    fprintf(assembly, "\t\t%%macro  push_regs    0\n");
    fprintf(assembly, "\t\tpush      rax\n");
    fprintf(assembly, "\t\tpush      rbx\n");
    fprintf(assembly, "\t\tpush      rcx\n");
    fprintf(assembly, "\t\tpush      rdx\n");
    // fprintf(assembly, "\t\tpush      rsp\n");
    // fprintf(assembly, "\t\tpush      rbp\n");
    fprintf(assembly, "\t\tpush      rsi\n");
    fprintf(assembly, "\t\tpush      rdi\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

    fprintf(assembly, "\t\t;macro to restore all registers\n");
    fprintf(assembly, "\t\t%%macro  pop_regs    0\n");
    fprintf(assembly, "\t\tpop      rdi\n");
    fprintf(assembly, "\t\tpop      rsi\n");
    // fprintf(assembly, "\t\tpop      rbp\n");
    // fprintf(assembly, "\t\tpop      rsp\n");
    fprintf(assembly, "\t\tpop      rdx\n");
    fprintf(assembly, "\t\tpop      rcx\n");
    fprintf(assembly, "\t\tpop      rbx\n");
    fprintf(assembly, "\t\tpop      rax\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

    fprintf(assembly, "\t\t;macro to align RSP\n");
    fprintf(assembly, "\t\t%%macro  rsp_align    0\n");
    fprintf(assembly, "\t\tpush      rbx\n");
    fprintf(assembly, "\t\tpush      rdx\n");
    fprintf(assembly, "\t\tpush      rax\n");
    fprintf(assembly, "\t\txor       rdx , rdx\n");
    fprintf(assembly, "\t\tmov       rax , rsp\n");
    fprintf(assembly, "\t\tmov       rbx , 16\n");
    fprintf(assembly, "\t\tidiv      rbx\n");
    fprintf(assembly, "\t\tmov       rbx , rdx\n");
    fprintf(assembly, "\t\tpop       rax\n");
    fprintf(assembly, "\t\tpop       rdx\n");
    fprintf(assembly, "\t\tsub       rsp , rbx\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

    fprintf(assembly, "\t\t;macro to re-align RSP\n");
    fprintf(assembly, "\t\t%%macro  rsp_realign    0\n");
    fprintf(assembly, "\t\tadd      rsp , rbx\n");
    fprintf(assembly, "\t\tpop      rbx\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");
}

void codegen_assgn_stmt(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |   ASSIGN   |    lhs   |    rhs   |    NULL   |
    +------------+----------+----------+-----------+
    */
    char* nameRHS = ir->left_op;      // temporary variable name for rhs
    char* nameLHS = ir->result;      // temporary variable name for rhs  
}

void codegen_logical(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of AND, OR
    */   
}

void codegen_input(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |  GET_VALUE |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    */
}

void codegen_output(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    PRINT   |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    */
}

void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of ADD, SUB, MUL, DIv
    */
}

void codegen_relational(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of LT, LTE, GT, GTE, EQ, NEQ
    */
}

void codegen_switch(ir_code_node* ir, func_entry* local_ST)
{
    
    
}

void codegen_jump(ir_code_node* ir, func_entry* local_ST)
{
    
    

    
}

void codegen_func(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |  fn_name |    NULL  |    NULL   |
    +------------+----------+----------+-----------+
    */
}

void codegen_conditional(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for conditional

    /* Handling the symbol table ops*/
}

void codegen_label(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void codegen_mem_read_ST(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void codegen_mem_write_ST(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void codegen_mem_read(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void codegen_mem_write(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void starter(FILE* assembly_file,ir_code* IR)
{

    assembly = assembly_file;
    // assembly = fopen("assembly_try.asm","w");

    if(!assembly)
        printf("[-] Error opening assembly_try.asm!\n");
    
    else
    {
        printf("[+] assembly_try.asm opened!\n");

        // Write the external functions and start data section for assembly
        fprintf(assembly, "; Group-20\n");
        fprintf(assembly, "; ---------------------------------\n");
        fprintf(assembly, "; 1. Rajan Sahu       2019B4A70572P\n");
        fprintf(assembly, "; 2. Yash Goyal       2019B4A70638P\n");
        fprintf(assembly, "; 3. Ayush Agarwal    2019B4A70652P\n");
        fprintf(assembly, "; 4. Vasu Swaroop     2019B4A70656P\n");
        fprintf(assembly, "; 5. A Sudarshan      2019B4A70744P\n");
        fprintf(assembly, "; ---------------------------------\n");
        fprintf(assembly, "extern printf, scanf, exit\n");
        fprintf(assembly, "\t\t; Data declaration such as zero and format specifiers for print/scan\n");
        fprintf(assembly, "\t\tsection      .data\n");

        // Write down all the format specifiers reqd
        fprintf(assembly, "\t\tfmt_spec_int_in: db \"%%d\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_int_out: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_real_in: db \"%%4f\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_real_out: db \"%%4f\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_string: db \"%%s\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool_in: db \"%%d\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool_out: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\ttrue: db \"true\", 10, 0\n");
        fprintf(assembly, "\t\ttrue_len: equ $ - true\n");
        fprintf(assembly, "\t\tfalse: db \"false\", 10, 0\n");
        fprintf(assembly, "\t\tfalse_len: equ $ - false\n");
        fprintf(assembly, "\t\tzero: equ 0\n");

        /* Data declaration of various types to be done by going thro each entry of the symbol table*/

        fprintf(assembly, "\n\n\t\tsection      .text\n");
        macros_starter();               // Define the macros
        fprintf(assembly, "\t\tglobal main\n");
        // fprintf(assembly, "main:\n");

        printf("[+] ASM file updated!\n");

    }
    ir_code_node *IR_head = IR->head;
    var_record* func_curr;
    func_entry* local_ST;

    // Go through each entry of the IR quadruple
    while(IR_head)
    {
        if(IR_head->operator==FUNC){ //if you reach FUNC --> then change the context
            
            //in case it is main --we have to search for DRIVER
            if (!strcmp(IR_head->result,"main")) local_ST = find_module_global("DRIVER");
            else local_ST = find_module_global(IR_head->result);
            func_curr = local_ST->func_root; //once we get the local_ST then just locate the root
        }


        char* reach = IR_head->reach; 

        if(strcmp(reach,"")){ //if reach is not "" then find the local construct
                              //in the current local_ST
            func_curr = find_local_construct(local_ST->name,reach);
        }else {
            func_curr = local_ST->func_root;
        }

        local_ST->func_curr = func_curr;

        switch (IR_head->operator)
        {
        case ASSIGN:
            codegen_assgn_stmt(IR_head, local_ST);
            break;

        case GET_VALUE:
            codegen_input(IR_head, local_ST);
            break;
        
        case PRINT:
            codegen_output(IR_head, local_ST);
            break;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
            codegen_arithmetic(IR_head, local_ST);
            break;
        
        case LT:
        case LE:
        case GT:
        case GE:
        case EQ:
        case NEQ:
            codegen_relational(IR_head, local_ST);
            break;
        
        case OR:
        case AND:
            codegen_logical(IR_head, local_ST);
            break;
        
        case FUNC:
            codegen_func(IR_head, local_ST);
        case CALL:
        case PARA_IN:
        case PARA_OUT:
        case RET:
            break;
        
        case GOTO:
            codegen_jump(IR_head, local_ST);
            break;
        
        case IF:
            codegen_conditional(IR_head, local_ST);
            break;
        
        case LABEL:
            codegen_label(IR_head, local_ST);
            break;
        
        case MEMREAD_ST:
            codegen_mem_read_ST(IR_head, local_ST);
            break;

        case MEMWRITE_ST:
            codegen_mem_write_ST(IR_head, local_ST);
            break;

        case MEMREAD:
            codegen_mem_read(IR_head, local_ST);
            break;

        case MEMWRITE:
            codegen_mem_write(IR_head, local_ST);
            break;
        }
        IR_head = IR_head->next;
    }
    fprintf(assembly, "main_end:\n");
    fprintf(assembly, "\t\tretq");
    fclose(assembly);
}
