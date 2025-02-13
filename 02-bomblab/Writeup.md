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

### Strating GDB

```bash
gdb ./bomb
```

### 设置断点

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

### 运行

```bash
run
continue (or c)
```

```bash
step (or s) # Step into functions
next (or n) # Step over functions
```

### 输出

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

#### Format

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

#### Unit

Specifies the size of each unit. Common units include:

- b: Byte (1 byte)

- h: Halfword (2 bytes)

- w: Word (4 bytes)

- g: Giant word (8 bytes)

### 回退

```bash
backtrace (or bt)
```

### 退出

```bash
quit (or q)
```
### 反汇编

```bash
disas
```

### 分割窗口

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

### 准备

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

### phase_1

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

### phase_2

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

### phase_3

1) 查看汇编代码

```bash
(gdb) disas phase_3
Dump of assembler code for function phase_3:
   0x0000000000400f43 <+0>:     sub    $0x18,%rsp
   0x0000000000400f47 <+4>:     lea    0xc(%rsp),%rcx
   0x0000000000400f4c <+9>:     lea    0x8(%rsp),%rdx
   0x0000000000400f51 <+14>:    mov    $0x4025cf,%esi
   0x0000000000400f56 <+19>:    mov    $0x0,%eax
   0x0000000000400f5b <+24>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000400f60 <+29>:    cmp    $0x1,%eax
   0x0000000000400f63 <+32>:    jg     0x400f6a <phase_3+39>
   0x0000000000400f65 <+34>:    call   0x40143a <explode_bomb>
   0x0000000000400f6a <+39>:    cmpl   $0x7,0x8(%rsp)
   0x0000000000400f6f <+44>:    ja     0x400fad <phase_3+106>
   0x0000000000400f71 <+46>:    mov    0x8(%rsp),%eax
   0x0000000000400f75 <+50>:    jmp    *0x402470(,%rax,8)
   0x0000000000400f7c <+57>:    mov    $0xcf,%eax
   0x0000000000400f81 <+62>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f83 <+64>:    mov    $0x2c3,%eax
   0x0000000000400f88 <+69>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f8a <+71>:    mov    $0x100,%eax
   0x0000000000400f8f <+76>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f91 <+78>:    mov    $0x185,%eax
   0x0000000000400f96 <+83>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f98 <+85>:    mov    $0xce,%eax
   0x0000000000400f9d <+90>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f9f <+92>:    mov    $0x2aa,%eax
   0x0000000000400fa4 <+97>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400fa6 <+99>:    mov    $0x147,%eax
   0x0000000000400fab <+104>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fad <+106>:   call   0x40143a <explode_bomb>
   0x0000000000400fb2 <+111>:   mov    $0x0,%eax
   0x0000000000400fb7 <+116>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fb9 <+118>:   mov    $0x137,%eax
   0x0000000000400fbe <+123>:   cmp    0xc(%rsp),%eax
   0x0000000000400fc2 <+127>:   je     0x400fc9 <phase_3+134>
   0x0000000000400fc4 <+129>:   call   0x40143a <explode_bomb>
   0x0000000000400fc9 <+134>:   add    $0x18,%rsp
   0x0000000000400fcd <+138>:   ret
End of assembler dump.
```

2) 分析汇编代码

注意到地址 `0x4025cf`

```bash
(gdb) x/s 0x4025cf
0x4025cf:       "%d %d"
```

所以需要输入两个整数

分别会存入栈中的 `0x8(%rsp)` 和 `0xc(%rsp)` 中，可以直接输出来确认这一点

于是 `cmpl   $0x7,0x8(%rsp)` 的作用是是检查第一个数是否大于 7 ，大于就会爆炸

发现之后是一个根据第一个数跳转到不同地址的结构，类似跳转表的结构，可以直接猜测这段内容是一个 `switch` 语句

注意语句 `jmp    *0x402470(,%rax,8)` 是一个间接跳转，以 `rax` 寄存器的值乘 8 作为偏移量，跳转到 `0x402470 + 8 * rax` 所储存的地址处，而不是跳转到这个地址

我们可以通过 `x/8a 0x402470` 查看这个跳转表

```bash
(gdb) x/8a 0x402470
0x402470:       0x400f7c <phase_3+57>   0x400fb9 <phase_3+118>
0x402480:       0x400f83 <phase_3+64>   0x400f8a <phase_3+71>
0x402490:       0x400f91 <phase_3+78>   0x400f98 <phase_3+85>
0x4024a0:       0x400f9f <phase_3+92>   0x400fa6 <phase_3+99>
```

跳转后  `mov` 指令将对应的值存入 `eax` 寄存器，再与输入的第二个数比较，相等即可通过

而各个 case 对应的值是

```
case 0: 0xcf     (207)
case 1: 0x137    (311)
case 2: 0x2c3    (707)
case 3: 0x100    (256)
case 4: 0x185    (389)
case 5: 0xce      (206)
case 6: 0x2aa     (682)
case 7: 0x147     (327)
```

所以答案有 8 组，随意输入一个即可

或者直接一步一步用 gdb 跳转也可以轻松得到答案

3) 设置断点并运行，随便取一组答案输入

```
(gdb) r ans.txt
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Phase 1 defused. How about the next one?
That's number 2.  Keep going!
>> 0 207
Halfway there!
```

记得把答案加入 `ans.txt`

### phase_4

1) 查看汇编代码

```bash
(gdb) disas phase_4
Dump of assembler code for function phase_4:
   0x000000000040100c <+0>:     sub    $0x18,%rsp
   0x0000000000401010 <+4>:     lea    0xc(%rsp),%rcx
   0x0000000000401015 <+9>:     lea    0x8(%rsp),%rdx
   0x000000000040101a <+14>:    mov    $0x4025cf,%esi
   0x000000000040101f <+19>:    mov    $0x0,%eax
   0x0000000000401024 <+24>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000401029 <+29>:    cmp    $0x2,%eax
   0x000000000040102c <+32>:    jne    0x401035 <phase_4+41>
   0x000000000040102e <+34>:    cmpl   $0xe,0x8(%rsp)
   0x0000000000401033 <+39>:    jbe    0x40103a <phase_4+46>
   0x0000000000401035 <+41>:    call   0x40143a <explode_bomb>
   0x000000000040103a <+46>:    mov    $0xe,%edx
   0x000000000040103f <+51>:    mov    $0x0,%esi
   0x0000000000401044 <+56>:    mov    0x8(%rsp),%edi
   0x0000000000401048 <+60>:    call   0x400fce <func4>
   0x000000000040104d <+65>:    test   %eax,%eax
   0x000000000040104f <+67>:    jne    0x401058 <phase_4+76>
   0x0000000000401051 <+69>:    cmpl   $0x0,0xc(%rsp)
   0x0000000000401056 <+74>:    je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:    call   0x40143a <explode_bomb>
   0x000000000040105d <+81>:    add    $0x18,%rsp
   0x0000000000401061 <+85>:    ret    
End of assembler dump.
```

发现调用了 `func4` 函数，，再查看 `func4` 函数的汇编代码

```bash
(gdb) disas func4
Dump of assembler code for function func4:
   0x0000000000400fce <+0>:     sub    $0x8,%rsp
   0x0000000000400fd2 <+4>:     mov    %edx,%eax
   0x0000000000400fd4 <+6>:     sub    %esi,%eax
   0x0000000000400fd6 <+8>:     mov    %eax,%ecx
   0x0000000000400fd8 <+10>:    shr    $0x1f,%ecx
   0x0000000000400fdb <+13>:    add    %ecx,%eax
   0x0000000000400fdd <+15>:    sar    %eax
   0x0000000000400fdf <+17>:    lea    (%rax,%rsi,1),%ecx
   0x0000000000400fe2 <+20>:    cmp    %edi,%ecx
   0x0000000000400fe4 <+22>:    jle    0x400ff2 <func4+36>
   0x0000000000400fe6 <+24>:    lea    -0x1(%rcx),%edx
   0x0000000000400fe9 <+27>:    call   0x400fce <func4>
   0x0000000000400fee <+32>:    add    %eax,%eax
   0x0000000000400ff0 <+34>:    jmp    0x401007 <func4+57>
   0x0000000000400ff2 <+36>:    mov    $0x0,%eax
   0x0000000000400ff7 <+41>:    cmp    %edi,%ecx
   0x0000000000400ff9 <+43>:    jge    0x401007 <func4+57>
   0x0000000000400ffb <+45>:    lea    0x1(%rcx),%esi
   0x0000000000400ffe <+48>:    call   0x400fce <func4>
   0x0000000000401003 <+53>:    lea    0x1(%rax,%rax,1),%eax        
   0x0000000000401007 <+57>:    add    $0x8,%rsp
   0x000000000040100b <+61>:    ret
End of assembler dump.
```

2) 分析汇编代码

先查看输入格式

```bash
(gdb) x/s 0x4025cf
0x4025cf:       "%d %d"
```

也是需要输入两个整数，设为 `a` 和 `b`

之后比较 `a` 是否小于等于 14，如果不是就爆炸

之后调用 `func4` 函数，通过
```assembly
mov    $0xe,%edx
mov    $0x0,%esi
mov    0x8(%rsp),%edi
```
传入 `14`, `0`, `a` 三个参数

注意到 `func4` 是一个递归函数，其中的核心运算如下

```assembly
mov    %edx,%eax          ; eax = edx
sub    %esi,%eax          ; eax -= esi
mov    %eax,%ecx          ; ecx = eax
shr    $0x1f,%ecx         ; ecx = ecx >> 31 (逻辑右移, ecx = 0 or -1)
add    %ecx,%eax          ; eax += ecx
sar    %eax               ; eax = eax >> 1 (算术右移)
lea    (%rax,%rsi,1),%ecx ; ecx = rax + rsi
```

后续跳转代码一行一行模拟即可发现 `func4` 就等价于以下 C 代码

```c
int func4(int x, int y, int z) {
   int t = x - y;
   if (t < 0) t -= 1;
   t >>= 1;
   int mid = t + y;
   if (mid == z) return 0;
   if (mid < z) return 2 * func4(x, mid + 1, z) + 1;
   if (mid > z) return 2 * func4(mid - 1, y, z);
}
```

实际上是一个带修正的二分查找，我们的输入需要满足 `func4(14, 0, a) == b` ，容易找到合法的解： `7 0` ，或者还有其他答案

3) 设置断点并运行，输入正确的数字即可排除炸弹

```
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Phase 1 defused. How about the next one?
That's number 2.  Keep going!
Halfway there!
7 0
So you got that one.  Try this one.
```

### phase_5

1) 查看汇编代码

```bash
(gdb) disas phase_5
Dump of assembler code for function phase_5:
   0x0000000000401062 <+0>:     push   %rbx
   0x0000000000401063 <+1>:     sub    $0x20,%rsp
   0x0000000000401067 <+5>:     mov    %rdi,%rbx
   0x000000000040106a <+8>:     mov    %fs:0x28,%rax
   0x0000000000401073 <+17>:    mov    %rax,0x18(%rsp)
   0x0000000000401078 <+22>:    xor    %eax,%eax
   0x000000000040107a <+24>:    call   0x40131b <string_length>      
   0x000000000040107f <+29>:    cmp    $0x6,%eax
   0x0000000000401082 <+32>:    je     0x4010d2 <phase_5+112>        
   0x0000000000401084 <+34>:    call   0x40143a <explode_bomb>       
   0x0000000000401089 <+39>:    jmp    0x4010d2 <phase_5+112>        
   0x000000000040108b <+41>:    movzbl (%rbx,%rax,1),%ecx
   0x000000000040108f <+45>:    mov    %cl,(%rsp)
   0x0000000000401092 <+48>:    mov    (%rsp),%rdx
   0x0000000000401096 <+52>:    and    $0xf,%edx
   0x0000000000401099 <+55>:    movzbl 0x4024b0(%rdx),%edx
   0x00000000004010a0 <+62>:    mov    %dl,0x10(%rsp,%rax,1)
   0x00000000004010a4 <+66>:    add    $0x1,%rax
   0x00000000004010a8 <+70>:    cmp    $0x6,%rax
   0x00000000004010ac <+74>:    jne    0x40108b <phase_5+41>
   0x00000000004010ae <+76>:    movb   $0x0,0x16(%rsp)
   0x00000000004010b3 <+81>:    mov    $0x40245e,%esi
   0x00000000004010b8 <+86>:    lea    0x10(%rsp),%rdi
   0x00000000004010bd <+91>:    call   0x401338 <strings_not_equal>  
   0x00000000004010c2 <+96>:    test   %eax,%eax
   0x00000000004010c4 <+98>:    je     0x4010d9 <phase_5+119>        
   0x00000000004010c6 <+100>:   call   0x40143a <explode_bomb>       
   0x00000000004010cb <+105>:   nopl   0x0(%rax,%rax,1)
   0x00000000004010d0 <+110>:   jmp    0x4010d9 <phase_5+119>        
   0x00000000004010d2 <+112>:   mov    $0x0,%eax
   0x00000000004010d7 <+117>:   jmp    0x40108b <phase_5+41>
   0x00000000004010d9 <+119>:   mov    0x18(%rsp),%rax
   0x00000000004010de <+124>:   xor    %fs:0x28,%rax
   0x00000000004010e7 <+133>:   je     0x4010ee <phase_5+140>        
   0x00000000004010e9 <+135>:   call   0x400b30 <__stack_chk_fail@plt>
   0x00000000004010ee <+140>:   add    $0x20,%rsp
   0x00000000004010f2 <+144>:   pop    %rbx
   0x00000000004010f3 <+145>:   ret
End of assembler dump.
```

其中 `string_length` 和 `strings_not_equal` 可以顾名思义，但保险起见还是查看一下汇编代码

```bash
(gdb) disas string_length
Dump of assembler code for function string_length:
   0x000000000040131b <+0>:     cmpb   $0x0,(%rdi)
   0x000000000040131e <+3>:     je     0x401332 <string_length+23>
   0x0000000000401320 <+5>:     mov    %rdi,%rdx
   0x0000000000401323 <+8>:     add    $0x1,%rdx
   0x0000000000401327 <+12>:    mov    %edx,%eax
   0x0000000000401329 <+14>:    sub    %edi,%eax
   0x000000000040132b <+16>:    cmpb   $0x0,(%rdx)
   0x000000000040132e <+19>:    jne    0x401323 <string_length+8>
   0x0000000000401330 <+21>:    repz ret
   0x0000000000401332 <+23>:    mov    $0x0,%eax
   0x0000000000401337 <+28>:    ret
End of assembler dump.
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

2) 分析汇编代码

首先出现的两个字符串函数都没有问题

`mov    %fs:0x28,%rax` 和 `mov    %rax,0x18(%rsp)` 是为了设置栈保护，具体操作是将 `fs` 段寄存器中的 `0x28` 处的值存入 `rax` 寄存器，再将 `rax` 寄存器的值存入 `0x18(%rsp)` 处，如果栈被破坏，`fs` 段寄存器中的值会发生变化，从而在 `xor    %fs:0x28,%rax` 时会爆炸

之后需要输入一个长度为 6 的字符串，然后对字符串进行处理

我们发现剩下的部分是一个循环，每次取出一个字符，并通过 `and   $0xf,%edx` 取出字符的低 4 位，再通过 `movzbl 0x4024b0(%rdx),%edx` 查表，将查到的值存入 `0x10(%rsp,%rax,1)` 处

于是我们查看 `0x4024b0` 处的表

```bash
(gdb) x/s 0x4024b0
0x4024b0 <array.3449>:  "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?"
```

意外发现了其他的字符串，但是我们只需要前 16 个字符，也就是 `maduiersnfotvbyl` ，这个字符串是一个映射表，将输入的字符映射到另一个字符

最后通过 `strings_not_equal` 函数比较处理后的字符串和 `0x40245e` 处的字符串

```bash
(gdb) x/s 0x40245e
0x40245e:       "flyers"
```

所以我们只需要对应能映射到 `flyers` 的字符串即可，发现可以是 `ionefg` 

3) 设置断点并运行，输入正确的字符串即可排除炸弹

```
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Phase 1 defused. How about the next one?
That's number 2.  Keep going!
Halfway there!
So you got that one.  Try this one.
ionefg
Good work!  On to the next...
```

### phase_6

1) 查看汇编代码

```bash
(gdb) disas phase_6
Dump of assembler code for function phase_6:
   0x00000000004010f4 <+0>:     push   %r14
   0x00000000004010f6 <+2>:     push   %r13
   0x00000000004010f8 <+4>:     push   %r12
   0x00000000004010fa <+6>:     push   %rbp
   0x00000000004010fb <+7>:     push   %rbx
   0x00000000004010fc <+8>:     sub    $0x50,%rsp
   0x0000000000401100 <+12>:    mov    %rsp,%r13
   0x0000000000401103 <+15>:    mov    %rsp,%rsi
   0x0000000000401106 <+18>:    call   0x40145c <read_six_numbers>
   0x000000000040110b <+23>:    mov    %rsp,%r14
   0x000000000040110e <+26>:    mov    $0x0,%r12d
   0x0000000000401114 <+32>:    mov    %r13,%rbp
   0x0000000000401117 <+35>:    mov    0x0(%r13),%eax
   0x000000000040111b <+39>:    sub    $0x1,%eax
   0x000000000040111e <+42>:    cmp    $0x5,%eax
   0x0000000000401121 <+45>:    jbe    0x401128 <phase_6+52>
   0x0000000000401123 <+47>:    call   0x40143a <explode_bomb>
   0x0000000000401128 <+52>:    add    $0x1,%r12d
   0x000000000040112c <+56>:    cmp    $0x6,%r12d
   0x0000000000401130 <+60>:    je     0x401153 <phase_6+95>
   0x0000000000401132 <+62>:    mov    %r12d,%ebx
   0x0000000000401135 <+65>:    movslq %ebx,%rax
   0x0000000000401138 <+68>:    mov    (%rsp,%rax,4),%eax
   0x000000000040113b <+71>:    cmp    %eax,0x0(%rbp)
   0x000000000040113e <+74>:    jne    0x401145 <phase_6+81>
   0x0000000000401140 <+76>:    call   0x40143a <explode_bomb>
   0x0000000000401145 <+81>:    add    $0x1,%ebx
   0x0000000000401148 <+84>:    cmp    $0x5,%ebx
   0x000000000040114b <+87>:    jle    0x401135 <phase_6+65>
   0x000000000040114d <+89>:    add    $0x4,%r13
   0x0000000000401151 <+93>:    jmp    0x401114 <phase_6+32>
   0x0000000000401153 <+95>:    lea    0x18(%rsp),%rsi
   0x0000000000401158 <+100>:   mov    %r14,%rax
   0x000000000040115b <+103>:   mov    $0x7,%ecx
   0x0000000000401160 <+108>:   mov    %ecx,%edx
   0x0000000000401162 <+110>:   sub    (%rax),%edx
   0x0000000000401164 <+112>:   mov    %edx,(%rax)
   0x0000000000401166 <+114>:   add    $0x4,%rax
   0x000000000040116a <+118>:   cmp    %rsi,%rax
   0x000000000040116d <+121>:   jne    0x401160 <phase_6+108>
   0x000000000040116f <+123>:   mov    $0x0,%esi
   0x0000000000401174 <+128>:   jmp    0x401197 <phase_6+163>
   0x0000000000401176 <+130>:   mov    0x8(%rdx),%rdx
   0x000000000040117a <+134>:   add    $0x1,%eax
   0x000000000040117d <+137>:   cmp    %ecx,%eax
   0x000000000040117f <+139>:   jne    0x401176 <phase_6+130>
   0x0000000000401181 <+141>:   jmp    0x401188 <phase_6+148>
   0x0000000000401183 <+143>:   mov    $0x6032d0,%edx
   0x0000000000401188 <+148>:   mov    %rdx,0x20(%rsp,%rsi,2)
   0x000000000040118d <+153>:   add    $0x4,%rsi
   0x0000000000401191 <+157>:   cmp    $0x18,%rsi
   0x0000000000401195 <+161>:   je     0x4011ab <phase_6+183>
   0x0000000000401197 <+163>:   mov    (%rsp,%rsi,1),%ecx
   0x000000000040119a <+166>:   cmp    $0x1,%ecx
   0x000000000040119d <+169>:   jle    0x401183 <phase_6+143>
   0x000000000040119f <+171>:   mov    $0x1,%eax
   0x00000000004011a4 <+176>:   mov    $0x6032d0,%edx
   0x00000000004011a9 <+181>:   jmp    0x401176 <phase_6+130>
   0x00000000004011ab <+183>:   mov    0x20(%rsp),%rbx
   0x00000000004011b0 <+188>:   lea    0x28(%rsp),%rax
   0x00000000004011b5 <+193>:   lea    0x50(%rsp),%rsi
   0x00000000004011ba <+198>:   mov    %rbx,%rcx
   0x00000000004011bd <+201>:   mov    (%rax),%rdx
   0x00000000004011c0 <+204>:   mov    %rdx,0x8(%rcx)
   0x00000000004011c4 <+208>:   add    $0x8,%rax
   0x00000000004011c8 <+212>:   cmp    %rsi,%rax
   0x00000000004011cb <+215>:   je     0x4011d2 <phase_6+222>
   0x00000000004011cd <+217>:   mov    %rdx,%rcx
   0x00000000004011d0 <+220>:   jmp    0x4011bd <phase_6+201>
   0x00000000004011d2 <+222>:   movq   $0x0,0x8(%rdx)
   0x00000000004011da <+230>:   mov    $0x5,%ebp
   0x00000000004011df <+235>:   mov    0x8(%rbx),%rax
   0x00000000004011e3 <+239>:   mov    (%rax),%eax
   0x00000000004011e5 <+241>:   cmp    %eax,(%rbx)
   0x00000000004011e7 <+243>:   jge    0x4011ee <phase_6+250>
   0x00000000004011e9 <+245>:   call   0x40143a <explode_bomb>
   0x00000000004011ee <+250>:   mov    0x8(%rbx),%rbx
   0x00000000004011f2 <+254>:   sub    $0x1,%ebp
   0x00000000004011f5 <+257>:   jne    0x4011df <phase_6+235>
   0x00000000004011f7 <+259>:   add    $0x50,%rsp
   0x00000000004011fb <+263>:   pop    %rbx
   0x00000000004011fc <+264>:   pop    %rbp
   0x00000000004011fd <+265>:   pop    %r12
   0x00000000004011ff <+267>:   pop    %r13
   0x0000000000401201 <+269>:   pop    %r14
   0x0000000000401203 <+271>:   ret    
End of assembler dump.
```

2) 分析汇编代码

首先调用 `read_six_numbers` 函数，读入六个整数

由于一开始执行了 `mov    %rsp,%r13` ，所以 `0x0(%r13)` 就是输入的第一个数，所以

```assembly
mov    0x0(%r13),%eax
sub    $0x1,%eax
cmp    $0x5,%eax
jbe    0x401128 <phase_6+52>
call   0x40143a <explode_bomb>
```

比较的就是第一个输入的数 `x` 是否在 1 到 6 之间，不在就爆炸

是所以还能限制 `x >= 1` 是因为寄存器存储的是无符号数

我们注意到之后的代码是一个循环嵌套，每次取出一个数，比较是否在 1 到 6 之间，以及是否有重复的数，相当于检查输入的是否是一个 1 到 6 的排列

```assembly
<+95>:    lea    0x18(%rsp),%rsi
<+100>:   mov    %r14,%rax
<+103>:   mov    $0x7,%ecx
<+108>:   mov    %ecx,%edx
<+110>:   sub    (%rax),%edx
<+112>:   mov    %edx,(%rax)
<+114>:   add    $0x4,%rax
<+118>:   cmp    %rsi,%rax
<+121>:   jne    0x401160 <phase_6+108>
```

接下来的循环把每个数 `x` 变成了 `7 - x`

接下来注意到地址 `0x6032d0` ，我们查看一下（多尝试几遍格式）

```bash
(gdb) x/s 0x6032d0
0x6032d0 <node1>:       "L\001"
(gdb) x/x 0x6032d0
0x6032d0 <node1>:       0x0000014c
```

可以合理猜测这是一个链表的结构

再多输出几个内容

```bash
(gdb) x/24x 0x6032d0
0x6032d0 <node1>:       0x0000014c      0x00000001      0x006032e0      0x00000000
0x6032e0 <node2>:       0x000000a8      0x00000002      0x006032f0      0x00000000
0x6032f0 <node3>:       0x0000039c      0x00000003      0x00603300      0x00000000
0x603300 <node4>:       0x000002b3      0x00000004      0x00603310      0x00000000
0x603310 <node5>:       0x000001dd      0x00000005      0x00603320      0x00000000
0x603320 <node6>:       0x000001bb      0x00000006      0x00000000      0x00000000
```

刚好得到了全部的链表，本来应该根据地址不断跳转，但是这里好像直接把内容放在了一起

而链表的结构可以猜到是

```c
struct Node {
   int value;
   int index
   struct Node *next;
};
```

之后根据链表的猜测容易解读出代码的意思让转换后的排列对应链表的值递减

于是应该输入 `4 3 2 1 6 5` （转换后是 `3 4 5 6 1 2` ）

3) 设置断点并运行，输入正确的数字即可排除炸弹

```
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Phase 1 defused. How about the next one?
That's number 2.  Keep going!
Halfway there!
So you got that one.  Try this one.
Good work!  On to the next...
4 3 2 1 6 5
Congratulations! You've defused the bomb!
```
