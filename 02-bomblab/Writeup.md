# Bomb Lab

根据说明下载并解压后有三个文件： `bomb`, `bomb.c`, `README`

阅读 `bomb.c` 可以知道 main 函数可以通过 stdin 读取一个字符串（无参数）或者从文件中读取（有参数，参数为文件名），例如这里为
```
$ ./bomb ans.txt
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Phase 1 defused. How about the next one?
That's number 2.  Keep going!
Halfway there!
So you got that one.  Try this one.
Good work!  On to the next...
Curses, you've found the secret phase!
But finding it and solving it are quite different...
```

需要使用 gdb 工具分析 `bomb` 对应的汇编代码，找到需要输入的答案

## GDB 用法

下面是 GDB 的基本功能和用法

#### Strating GDB

```bash
gdb ./bomb
```

#### 设置断点

可以在某一行，某个地址或者某个函数设置断点

```bash
b function_name
b *address
b file.c:line_number
```

显示断点，可以看到断点的编号，地址，函数名，文件名和行号

```bash
i b
```

删除断点

```bash
delete n # we can get n from the output of i b
delete breadkpoints # delete all breakpoints
```

#### 运行

```bash
run
continue (or c)
```

```bash
step (or s) # Step into functions
next (or n) # Step over functions
```

#### 输出

输出变量
```bash
print var_name
```

检查地址
```bash
x/[number][format][unit] address address
```

例如：
```bash
x/8x $rsp # Examine 8 words in hex starting at the stack pointer
```

##### Format

- x: Hexadecimal

- d: Signed decimal

- u: Unsigned decimal

- t: Binary

- o: Octal

- a: Address (pointer)

- c: Character

- f: Floating point

- s: String

- i: Instruction (disassembly)

##### Unit

Specifies the size of each unit. Common units include:

- b: Byte (1 byte)

- h: Halfword (2 bytes)

- w: Word (4 bytes)

- g: Giant word (8 bytes)

#### 回退

```bash
backtrace (or bt)
```

#### 退出

```bash
quit (or q)
```
#### 反汇编

```bash
disas
```

#### 分割窗口

两个命令分别显示汇编代码和寄存器

```bash
layout asm
layout regs
```

## 运行环境

```
wsl
```

## 拆弹

#### 准备

1) 通过 `gdb` 运行 `bomb` ，并查看 main 函数汇编代码

```bash
>> gdb bomb
(gdb) disas main
Dump of assembler code for function main:
   0x0000000000400da0 <+0>:     push   %rbx
   0x0000000000400da1 <+1>:     cmp    $0x1,%edi
   0x0000000000400da4 <+4>:     jne    0x400db6 <main+22>
   0x0000000000400da6 <+6>:     mov    0x20299b(%rip),%rax        # 0x603748 <stdin@@GLIBC_2.2.5>
   0x0000000000400dad <+13>:    mov    %rax,0x2029b4(%rip)        # 0x603768 <infile>
   0x0000000000400db4 <+20>:    jmp    0x400e19 <main+121>
   0x0000000000400db6 <+22>:    mov    %rsi,%rbx
   0x0000000000400db9 <+25>:    cmp    $0x2,%edi
   0x0000000000400dbc <+28>:    jne    0x400df8 <main+88>
   0x0000000000400dbe <+30>:    mov    0x8(%rsi),%rdi
   0x0000000000400dc2 <+34>:    mov    $0x4022b4,%esi
   0x0000000000400dc7 <+39>:    call   0x400c10 <fopen@plt>
   0x0000000000400dcc <+44>:    mov    %rax,0x202995(%rip)        # 0x603768 <infile>
   0x0000000000400dd3 <+51>:    test   %rax,%rax
   0x0000000000400dd6 <+54>:    jne    0x400e19 <main+121>
   0x0000000000400dd8 <+56>:    mov    0x8(%rbx),%rcx
   0x0000000000400ddc <+60>:    mov    (%rbx),%rdx
   0x0000000000400ddf <+63>:    mov    $0x4022b6,%esi
   0x0000000000400de4 <+68>:    mov    $0x1,%edi
   0x0000000000400de9 <+73>:    call   0x400c00 <__printf_chk@plt>
   0x0000000000400dee <+78>:    mov    $0x8,%edi
   0x0000000000400df3 <+83>:    call   0x400c20 <exit@plt>
   0x0000000000400df8 <+88>:    mov    (%rsi),%rdx
   0x0000000000400dfb <+91>:    mov    $0x4022d3,%esi
   0x0000000000400e00 <+96>:    mov    $0x1,%edi
   0x0000000000400e05 <+101>:   mov    $0x0,%eax
   0x0000000000400e0a <+106>:   call   0x400c00 <__printf_chk@plt>
   0x0000000000400e0f <+111>:   mov    $0x8,%edi
   0x0000000000400e14 <+116>:   call   0x400c20 <exit@plt>
   0x0000000000400e19 <+121>:   call   0x4013a2 <initialize_bomb>
   0x0000000000400e1e <+126>:   mov    $0x402338,%edi
   0x0000000000400e23 <+131>:   call   0x400b10 <puts@plt>
   0x0000000000400e28 <+136>:   mov    $0x402378,%edi
   0x0000000000400e2d <+141>:   call   0x400b10 <puts@plt>
   0x0000000000400e32 <+146>:   call   0x40149e <read_line>
   0x0000000000400e37 <+151>:   mov    %rax,%rdi
   0x0000000000400e3a <+154>:   call   0x400ee0 <phase_1>
   0x0000000000400e3f <+159>:   call   0x4015c4 <phase_defused>
   0x0000000000400e44 <+164>:   mov    $0x4023a8,%edi
   0x0000000000400e49 <+169>:   call   0x400b10 <puts@plt>
   0x0000000000400e4e <+174>:   call   0x40149e <read_line>
   0x0000000000400e53 <+179>:   mov    %rax,%rdi
   0x0000000000400e56 <+182>:   call   0x400efc <phase_2>
   0x0000000000400e5b <+187>:   call   0x4015c4 <phase_defused>
   0x0000000000400e60 <+192>:   mov    $0x4022ed,%edi
   0x0000000000400e65 <+197>:   call   0x400b10 <puts@plt>
   0x0000000000400e6a <+202>:   call   0x40149e <read_line>
   0x0000000000400e6f <+207>:   mov    %rax,%rdi
   0x0000000000400e72 <+210>:   call   0x400f43 <phase_3>
   0x0000000000400e77 <+215>:   call   0x4015c4 <phase_defused>
   0x0000000000400e7c <+220>:   mov    $0x40230b,%edi
   0x0000000000400e81 <+225>:   call   0x400b10 <puts@plt>
   0x0000000000400e86 <+230>:   call   0x40149e <read_line>
   0x0000000000400e8b <+235>:   mov    %rax,%rdi
   0x0000000000400e8e <+238>:   call   0x40100c <phase_4>
   0x0000000000400e93 <+243>:   call   0x4015c4 <phase_defused>
   0x0000000000400e98 <+248>:   mov    $0x4023d8,%edi
   0x0000000000400e9d <+253>:   call   0x400b10 <puts@plt>
   0x0000000000400ea2 <+258>:   call   0x40149e <read_line>
   0x0000000000400ea7 <+263>:   mov    %rax,%rdi
   0x0000000000400eaa <+266>:   call   0x401062 <phase_5>
   0x0000000000400eaf <+271>:   call   0x4015c4 <phase_defused>
   0x0000000000400eb4 <+276>:   mov    $0x40231a,%edi
   0x0000000000400eb9 <+281>:   call   0x400b10 <puts@plt>
   0x0000000000400ebe <+286>:   call   0x40149e <read_line>
   0x0000000000400ec3 <+291>:   mov    %rax,%rdi
   0x0000000000400ec6 <+294>:   call   0x4010f4 <phase_6>
   0x0000000000400ecb <+299>:   call   0x4015c4 <phase_defused>
   0x0000000000400ed0 <+304>:   mov    $0x0,%eax
   0x0000000000400ed5 <+309>:   pop    %rbx
   0x0000000000400ed6 <+310>:   ret    
End of assembler dump.
```

或者可以直接用 `objdump` 导出到 `bomb.asm` 文件方便查看

```bash
objdump -d bomb > bomb.asm
```

#### phase_1

1) 查看 `phase_1` 函数的汇编代码

```bash
(gdb) disas phase_1
Dump of assembler code for function phase_1:
   0x0000000000400ee0 <+0>:     sub    $0x8,%rsp
   0x0000000000400ee4 <+4>:     mov    $0x402400,%esi
   0x0000000000400ee9 <+9>:     call   0x401338 <strings_not_equal>
   0x0000000000400eee <+14>:    test   %eax,%eax
   0x0000000000400ef0 <+16>:    je     0x400ef7 <phase_1+23>
   0x0000000000400ef2 <+18>:    call   0x40143a <explode_bomb>
   0x0000000000400ef7 <+23>:    add    $0x8,%rsp
   0x0000000000400efb <+27>:    ret
End of assembler dump.
```

2) 发现爆炸是调用 `explode_bomb` 函数，先设置断点 `b explode_bomb` ，防止爆炸
3) 分析汇编代码，发现是比较输入字符串和 `0x402400` 地址的字符串是否相等

或者更严谨可以再查看 `strings_not_equal` 函数的汇编代码

```bash
(gdb) disas strings_not_equal
Dump of assembler code for function strings_not_equal:
   0x0000000000401338 <+0>:     push   %r12
   0x000000000040133a <+2>:     push   %rbp
   0x000000000040133b <+3>:     push   %rbx
   0x000000000040133c <+4>:     mov    %rdi,%rbx
   0x000000000040133f <+7>:     mov    %rsi,%rbp
   0x0000000000401342 <+10>:    call   0x40131b <string_length>
   0x0000000000401347 <+15>:    mov    %eax,%r12d
   0x000000000040134a <+18>:    mov    %rbp,%rdi
   0x000000000040134d <+21>:    call   0x40131b <string_length>
   0x0000000000401352 <+26>:    mov    $0x1,%edx
   0x0000000000401357 <+31>:    cmp    %eax,%r12d
   0x000000000040135a <+34>:    jne    0x40139b <strings_not_equal+99>
   0x000000000040135c <+36>:    movzbl (%rbx),%eax
   0x000000000040135f <+39>:    test   %al,%al
   0x0000000000401361 <+41>:    je     0x401388 <strings_not_equal+80>
   0x0000000000401363 <+43>:    cmp    0x0(%rbp),%al
   0x0000000000401366 <+46>:    je     0x401372 <strings_not_equal+58>
   0x0000000000401368 <+48>:    jmp    0x40138f <strings_not_equal+87>
   0x000000000040136a <+50>:    cmp    0x0(%rbp),%al
   0x000000000040136d <+53>:    nopl   (%rax)
   0x0000000000401370 <+56>:    jne    0x401396 <strings_not_equal+94>
   0x0000000000401372 <+58>:    add    $0x1,%rbx
   0x0000000000401376 <+62>:    add    $0x1,%rbp
   0x000000000040137a <+66>:    movzbl (%rbx),%eax
   0x000000000040137d <+69>:    test   %al,%al
   0x000000000040137f <+71>:    jne    0x40136a <strings_not_equal+50>
   0x0000000000401381 <+73>:    mov    $0x0,%edx
   0x0000000000401386 <+78>:    jmp    0x40139b <strings_not_equal+99>
   0x0000000000401388 <+80>:    mov    $0x0,%edx
   0x000000000040138d <+85>:    jmp    0x40139b <strings_not_equal+99>
   0x000000000040138f <+87>:    mov    $0x1,%edx
   0x0000000000401394 <+92>:    jmp    0x40139b <strings_not_equal+99>
   0x0000000000401396 <+94>:    mov    $0x1,%edx
   0x000000000040139b <+99>:    mov    %edx,%eax
   0x000000000040139d <+101>:   pop    %rbx
   0x000000000040139e <+102>:   pop    %rbp
   0x000000000040139f <+103>:   pop    %r12
   0x00000000004013a1 <+105>:   ret    
End of assembler dump.
```

4) 所以我们需要找到 `0x402400` 地址的字符串，可以通过 `x/s 0x402400` 查看

```bash
(gdb) x/s 0x402400
0x402400:       "Border relations with Canada have never been better."
```

5) 在 `phase_2` 入口设置断点后，运行输入字符串即可

```
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
>> Border relations with Canada have never been better.
Phase 1 defused. How about the next one?
```

成功拆除

6) 我们可以把答案存入 `ans.txt` 文件中，这些之后不用重复输入，运行可以直接 `r ans.txt`

#### phase_2

1) 先查看汇编代码

```bash
(gdb) disas phase_2
Dump of assembler code for function phase_2:
   0x0000000000400efc <+0>:     push   %rbp
   0x0000000000400efd <+1>:     push   %rbx
   0x0000000000400efe <+2>:     sub    $0x28,%rsp
   0x0000000000400f02 <+6>:     mov    %rsp,%rsi
   0x0000000000400f05 <+9>:     call   0x40145c <read_six_numbers>
   0x0000000000400f0a <+14>:    cmpl   $0x1,(%rsp)
   0x0000000000400f0e <+18>:    je     0x400f30 <phase_2+52>
   0x0000000000400f10 <+20>:    call   0x40143a <explode_bomb>
   0x0000000000400f15 <+25>:    jmp    0x400f30 <phase_2+52>
   0x0000000000400f17 <+27>:    mov    -0x4(%rbx),%eax
   0x0000000000400f1a <+30>:    add    %eax,%eax
   0x0000000000400f1c <+32>:    cmp    %eax,(%rbx)
   0x0000000000400f1e <+34>:    je     0x400f25 <phase_2+41>
   0x0000000000400f20 <+36>:    call   0x40143a <explode_bomb>
   0x0000000000400f25 <+41>:    add    $0x4,%rbx
   0x0000000000400f29 <+45>:    cmp    %rbp,%rbx
   0x0000000000400f2c <+48>:    jne    0x400f17 <phase_2+27>
   0x0000000000400f2e <+50>:    jmp    0x400f3c <phase_2+64>
   0x0000000000400f30 <+52>:    lea    0x4(%rsp),%rbx
   0x0000000000400f35 <+57>:    lea    0x18(%rsp),%rbp
   0x0000000000400f3a <+62>:    jmp    0x400f17 <phase_2+27>
   0x0000000000400f3c <+64>:    add    $0x28,%rsp
   0x0000000000400f40 <+68>:    pop    %rbx
   0x0000000000400f41 <+69>:    pop    %rbp
   0x0000000000400f42 <+70>:    ret    
End of assembler dump.

(gdb) disas read_six_numbers
Dump of assembler code for function read_six_numbers:
   0x000000000040145c <+0>:     sub    $0x18,%rsp
   0x0000000000401460 <+4>:     mov    %rsi,%rdx
   0x0000000000401463 <+7>:     lea    0x4(%rsi),%rcx
   0x0000000000401467 <+11>:    lea    0x14(%rsi),%rax
   0x000000000040146b <+15>:    mov    %rax,0x8(%rsp)
   0x0000000000401470 <+20>:    lea    0x10(%rsi),%rax
   0x0000000000401474 <+24>:    mov    %rax,(%rsp)
   0x0000000000401478 <+28>:    lea    0xc(%rsi),%r9
   0x000000000040147c <+32>:    lea    0x8(%rsi),%r8
   0x0000000000401480 <+36>:    mov    $0x4025c3,%esi
   0x0000000000401485 <+41>:    mov    $0x0,%eax
   0x000000000040148a <+46>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x000000000040148f <+51>:    cmp    $0x5,%eax
   0x0000000000401492 <+54>:    jg     0x401499 <read_six_numbers+61>
   0x0000000000401494 <+56>:    call   0x40143a <explode_bomb>
   0x0000000000401499 <+61>:    add    $0x18,%rsp
   0x000000000040149d <+65>:    ret    
End of assembler dump.
```

2) 分析输入函数

`read_six_number` 函数中出现了一个地址 `0x4025c3`，我们可以通过 `x/s 0x4025c3` 查看

```bash
(gdb) x/s 0x4025c3
0x4025c3:       "%d %d %d %d %d %d"
```

再结合名称可以判断这个函数需要读入 6 个整数

阅读 `read_six_numbers` 函数汇编代码，发现读到的数字个数存在 `%eax` 中，如果小于等于 5 就会爆炸

而如果输入了超过 6 个数，由于 `sscanf` 没有限制输入个数，如果超过 6 个数会继续写入内存，从而造成栈溢出，导致爆炸

3) 分析 `phase_2` 函数

在输入正确后 `cmpl   $0x1,(%rsp)` 检查栈顶元素，也就是输入的第一个数是否为 1，如果不是就爆炸

再跳转到

```assembly
lea    0x4(%rsp),%rbx
lea    0x18(%rsp),%rbp
```

把 `rbx` 初始化为输入的第 2 个数，`rbp` 初始化为输入的第 6 个数

之后进入循环，每次检查 `rbx` 指向的数是否是 `rbx` 的前一个数的两倍，如果不是就爆炸

所以显然需要输入 `1 2 4 8 16 32`

4) 设置断点并运行，输入正确的数字即可排除炸弹

```
(gdb) r ans.txt
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Phase 1 defused. How about the next one?
>> 1 2 4 8 16 32
That's number 2.  Keep going!
```

