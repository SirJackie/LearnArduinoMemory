# 学习Arduino内存分配原理

## 1. 起始终止点

- 之前我想当然认为，Arduino SRAM有2048 Bytes内存，所以内存地址是0到2047。

- 但是，我在查阅英文文章之后，发现：

  > Note than in a 328, RAM starts at address 0x100 (256 DEC), and ends at address 0x8FF (2303 DEC).
  >
  > Source：https://forum.arduino.cc/t/trying-to-make-sense-of-ram-usage/622666

- 原来，Arduino内存地址时256到2303。（2303-256=2047，算上256本身刚好2048个格子）

## 2. 头朝上？

- 阅读这篇文章的时候：https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory，看到这张图：

  <img src="Note.assets/learn_arduino_Stack_Operation.gif" alt="learn_arduino_Stack_Operation" style="zoom:50%;" />

- 错以为：

  - 这张图是从上往下看。
  - Stack Top是从256开始，Stack Bottom不定。
  - Heap Bottom是从2303减去Static Data开始，Heap Top不定。
  - `freeMemory = Heap Top - Stack Bottom`这样子。

- 实际上：

  - **这张图**是**从下往上看**！
  - Static Data从256开始。
  - Heap Top从`( 256 + lengthOf( Static Data ) )`开始，Heap Bottom不定。
  - Stack Bottom从2303开始，Stack Top不定。
  - `freeMemory = Stack Top - Heap Bottom`这样才对。

- 所以，之前的代码写错了。

## 3. 几种不同的Memory获取方式

第一种：来自Adafruit教程：Measuring Memory Usage；链接：https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory

```C++
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
```

第二种：来自Arduino官方文档：Arduino Memory Guide里面的SRAM Memory Measurement；链接：https://docs.arduino.cc/learn/programming/memory-guide/

**measure SRAM usage in AVR-based Arduino® boards**:

```C++
void display_freeram() {
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0  
    ? (int)&__heap_start : (int) __brkval);  
}
```

**measure SRAM usage in ARM-based Arduino® boards**:

```C++
extern "C" char* sbrk(int incr);

void display_freeram(){
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

int freeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
```

第三种：Arduino Forum 上用户 [RayLivingston](https://forum.arduino.cc/u/RayLivingston) 的帖子：`Trying To Make Sense of RAM Usage....`；链接：https://forum.arduino.cc/t/trying-to-make-sense-of-ram-usage/622666

TA提供了更底层的办法，但是好像他的理解跟我一样有些问题，下面有人纠正TA。无论如何，这是他的代码：

```C++
void showMemory(const char *s)
{
 char __stack = 0;
 
 Serial.println(s);
 Serial.print("__brkval=");
 Serial.println((unsigned int)__brkval);
 Serial.print("__flp=");
 Serial.println((unsigned int)__flp);
 Serial.print("__stack=");
 Serial.println((unsigned int)&__stack);
 Serial.print("stack size=");
 Serial.println(RAM_end - (unsigned int)&__stack);
 Serial.print("Heap size=");
 Serial.println((unsigned int)__brkval - RAM_start);
 
 
 struct __freelist* current;
 int total = 0;
 for (current = __flp; current; current = current->nx)
 {
 total += 2; /* Add two bytes for the memory block's header  */
 total += (int) current->sz;
 Serial.print("mblk: sz=");
 Serial.print((unsigned int)current->sz);
 Serial.print(" nx=");
 Serial.println((unsigned int)current->nx);
 Serial.print("Total: ");
 Serial.println(total);
 }
 Serial.println("\n");
}
```

第四种：ChatGPT提供的方法：

```C++
extern unsigned int __heap_start;
extern void *__brkval;

void setup() {
  Serial.begin(9600);
  delay(1000);

  // 获取堆顶部地址
  unsigned int topOfHeap;
  if ((int)__brkval == 0) {
    topOfHeap = (unsigned int)&__heap_start;
  } else {
    topOfHeap = (unsigned int)__brkval;
  }

  Serial.print("Top of heap address: 0x");
  Serial.println(topOfHeap, HEX);
}

void loop() {
  // 你的循环代码
}

```

-----

### **看完这几种方法，我通过实验，搞明白了：不同的built-in variable和内存分布的关系：**

一共有这几个变量：

```C++
extern char *__brkval;
extern char *__malloc_heap_start;
extern char *__heap_start;
extern char *__flp;
char stackTop = 0; (unsigned int) &stackTop;
```

我写的详细解释：

- `__heap_start`: A Seperator, which address is at the top of heap(about 454), which value is 12;
- `__malloc_heap_start`: A Static Variable, which address is at 258 (Static Variable Section), which value is the top of heap(about 454);
- `__brkval`: A Counter, which address is at 450(2 uint16_t before the top of the heap 454), which value is initially 0, then becomes the bottom of the heap(468 in this example) after a new[] or malloc() operation.
- `__flp`: A Counter, which address is at 452(1 uint16_t before the top of the heap 454), which value is initially 0, then becomes the top of the heap(454 in this example), and it reassigned to 0 after all dynaArrays delete[]d, however, __brkval won't reassigned to 0 after all dynaArrays delete[]d, but 454(Heap Top).

至于实际运用，可以看看我的这两段代码：

```C++
extern int stopOptimization;
extern char *__brkval;
extern char *__malloc_heap_start;
extern char *__heap_start;
extern char *__flp;

int GetStackTop() {
  // Latest Local Variable in Lastest Called Function = Stack Top
  char stackTop;

  // Must subtract an external int to disable compiler optimization.
  return &stackTop - stopOptimization;
}

int GetHeapBottom() {
  return (unsigned int) __brkval;
}

void setup() {
  Serial.begin(115200);

  Serial.println((int) __heap_start);
  Serial.println((int)&__heap_start);
  Serial.println((int) __malloc_heap_start);
  Serial.println((int)&__malloc_heap_start);
  Serial.println((int) __brkval);
  Serial.println((int)&__brkval);
  Serial.println(F("------------------------"));

  int* dynaArray = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray[i] = 127;
  }

  Serial.println((int) __heap_start);
  Serial.println((int)&__heap_start);
  Serial.println((int) __malloc_heap_start);
  Serial.println((int)&__malloc_heap_start);
  Serial.println((int) __brkval);
  Serial.println((int)&__brkval);
  Serial.println(F("------------------------"));

  delete[] dynaArray;

  Serial.println((int) __heap_start);
  Serial.println((int)&__heap_start);
  Serial.println((int) __malloc_heap_start);
  Serial.println((int)&__malloc_heap_start);
  Serial.println((int) __brkval);
  Serial.println((int)&__brkval);
  Serial.println(F("------------------------"));

  /*
  ** Result:
  (int) __heap_start
  (int)&__heap_start
  (int) __malloc_heap_start
  (int)&__malloc_heap_start
  (int) __brkval
  (int)&__brkval
  ------------------------
  12
  454
  454
  258
  0
  450
  ------------------------
  12
  454
  454
  258
  468
  450
  ------------------------
  12
  454
  454
  258
  454
  450
  ------------------------
  __heap_start        : A Seperator, which address is at the top of heap(about 454), which value is 12;
  __malloc_heap_start : A Static Variable, which address is at 258 (Static Variable Section),
                        which value is the top of heap(about 454);
  __brkval            : A Counter, which address is at 450(one uint32_t before the top of the heap 454),
                        which value is initialy 0, then becomes the bottom of the heap(468 in this example)
                        after a new[] or malloc() operation.
  ------------------------
  */
}

void loop() {

}

```

```C++
extern int stopOptimization;
extern char *__brkval;
extern char *__malloc_heap_start;
extern char *__heap_start;
extern char *__flp;

int GetStackTop() {
  // Latest Local Variable in Lastest Called Function = Stack Top
  char stackTop;

  // Must subtract an external int to disable compiler optimization.
  return &stackTop - stopOptimization;
}

int GetHeapBottom() {
  return (unsigned int) __brkval;
}

void setup() {
  Serial.begin(115200);

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  int* dynaArray = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray[i] = 127;
  }

  int* dynaArray2 = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray2[i] = 127;
  }

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  delete[] dynaArray;

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  delete[] dynaArray2;

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  /*
  ** Result:
  (int)&__heap_start
  (int) __brkval
  (int) __flp
  (int)&__flp
  ------------------------
  454
  0
  0
  452
  ------------------------
  454
  482
  0
  452
  ------------------------
  454
  482
  454
  452
  ------------------------
  454
  454
  0
  452
  ------------------------
  __heap_start        : A Seperator, which address is at the top of heap(about 454), which value is 12;
  __malloc_heap_start : A Static Variable, which address is at 258 (Static Variable Section),
                        which value is the top of heap(about 454);
  __brkval            : A Counter, which address is at 450(2 uint16_t before the top of the heap 454),
                        which value is initially 0, then becomes the bottom of the heap(468 in this example)
                        after a new[] or malloc() operation.
  __flp               : A Counter, which address is at 452(1 uint16_t before the top of the heap 454),
                        which value is initially 0, then becomes the top of the heap(454 in this example),
                        and it reassigned to 0 after all dynaArrays delete[]d,
                        however, __brkval won't reassigned to 0 after all dynaArrays delete[]d, but 454(Heap Top).
  ------------------------
  */
}

void loop() {

}

```

