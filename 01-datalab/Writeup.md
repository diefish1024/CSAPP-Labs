# Data Lab

默认只能使用 `! ~ & ^ | + << >>` 和不超过 `0xFF` 的常数

#### bitXor

要求：用 `~` 和 `&` 表示出两个数的按位异或

异或可以视为不进位加法，故可以先考虑一位的情况，容易猜出结果(?)

再用取反和与表示出或，得到

```cpp
res = ~(x & y) & ~(~x & ~y)
```

#### tmin

要求：直接返回 `int` 下最小的二进制数

根据补码的规则，最小的直接可以表示为 `1 << 31` 

#### isTmax

要求：返回 `x` 是否是 `int` 下最大的二进制数，只能使用 `! ~ & ^ | +` 

即判断 `x` 是否是 `~(1 << 31)` ，但由于不能使用位移运算符，故需要从溢出等性质入手

观察到 `x + ~x = -1` ，即 `(x + 1) + ~x = 0` ，而其他满足这个性质的数还有 `0` ，需要特判不等于零，因此

```cpp
res = !((x + 1) + ~x) & !!x
```

#### allOddBits

要求：返回 `x` 的所有奇数位是否都是 `1` （从 `0` 开始计数）

通过位移用 `0xAA` 拼出所有奇数位都是 `1` 的 `0xAAAAAAAA` 即可

#### negate

要求：返回 `-x` 

取反加 $1$ 

#### isAsciiDight

要求：判断 `x` 是否在 `0x30` 到 `0x39` 之间（对应的 ASCII 码是数字）

通过取反加一得到减法再判断符号位即可

#### conditional

要求：实现三目运算符 `x ? y : z` 

直接 `(y & x) | (z & ~x)` 

#### isLessOrEqual

要求：判断是否有 `x <= y` 

需要考虑溢出的情况，所以需要同时判断 `y + (-x)` 的符号位以及 `x,y` 自身符号位是否相同

```cpp
int isLessOrEqual(int x, int y) {
  int sigx = x >> 31 & 1;
  int sigy = y >> 31 & 1;
  int dif_s = sigx ^ sigy;

  int xinv = ~x + 1;
  int dif = y + xinv;
  int sig = dif >> 31 & 1;

  return (dif_s & sigx) | (!dif_s & !sig);
}
```

#### logicalNeg

要求：返回 `!x` 

即需要把非零的数映射到 $0$ ，把 $0$ 映射成 $1$ 

可以利用除了零以外其他数取反加一以后符号位都会改变的特性，所以可以

```cpp
return ((x | (~x+1)) >> 31) + 1;
```

#### howManyBits

要求：返回要表示 `x` 用补码表示最少需要的位数

正数和负数需要分开处理，正数只需要找最高位的 `1` ，而负数是找最高位的 `0` ，所以考虑在负数时把 `x` 取反，那么就可以合成一种情况，于是 `x = x ^ (x >> 31)` 

之后找到最高位的 `1` 可以考虑通过二分的希望以此找出最高位数二进制表示下的一位

```cpp
int howManyBits(int x) {
  int bit;
  int res = 1;
  x = x ^ (x >> 31);

  bit = !!(x >> 16) << 4;
  res = res + bit;
  x = x >> bit;

  bit = !!(x >> 8) << 3;
  res = res + bit;
  x = x >> bit;

  bit = !!(x >> 4) << 2;
  res = res + bit;
  x = x >> bit;

  bit = !!(x >> 2) << 1;
  res = res + bit;
  x = x >> bit;

  bit = !!(x >> 1);
  res = res + bit;
  x = x >> bit;

  return x + res;
}
```

#### floatScale2

要求：传入浮点数的位级表示 `uf` ，返回 `2*uf` 的位级表示

按照浮点数的格式拆成三个部分

```cpp
int s = (uf >> 31) & 1;
int e = (uf >> 23) & 0xFF;
int f = uf ^ (s << 31) ^ (e << 23);
```

分规格化、非规格化、特殊值三种情况讨论

```cpp
if(!e) return (s << 31) | (f << 1);
if(!(e ^ 0xFF)) return uf;
return (s << 31) | ((e + 1) << 23) | f;
```

#### floatFloat2Int

要求：返回 `(int)uf` ，超出范围或者特殊值需要返回 `0x80000000u`

注意分类讨论，直接考虑 `e` 的范围可以省去特住值的讨论，因为此时 `e` 一定足够大

注意正负数的上下限不一样

在 `e` 足够小的时候直接返回 `0` 

```cpp
int floatFloat2Int(unsigned uf) {
  int s = (uf >> 31) & 1;
  int e = (uf >> 23) & 0xFF;
  int f = uf ^ (s << 31) ^ (e << 23);
  if(!(e | f)) return 0;
  e = e - 0x7F;
  if(e > 30 + ((!f) & s)) return 0x80000000u;
  if(23 - e >= 32) return 0;
  f = ((1 << 23) | f) >> (23 - e);
  return s ? -f : f;
}
```

#### floatPower2

要求：返回 $2.0^{x}$ 浮点数的位级表示

注意细节

```cpp
unsigned floatPower2(int x) {
  x = x + 0x7f;
  if(x < 0) return 0;
  return (x < 0xFF ? x : 0xFF) << 23;
}
```

