# Bomb Lab

`bomb` 中有一个有 7 个阶段的炸弹，每个阶段需要输入特定的答案才能不让炸弹爆炸

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
break function_name
break *address
break file.c:line_number
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

