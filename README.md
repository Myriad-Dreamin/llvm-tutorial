
## LLVM-tutorial

kaleidoscope是官方tutorial的玩具语言，这里的kaleidoscope其实和tutorial的关系不大。。

### Installation

下载：
```bash
~$ git clone https://github.com/Myriad-Dreamin/llvm-tutorial.git
~$ cd llvm-tutorial
~$ git submodule update --init
```

编译LLVM
```bash
~$ cd lib
~/lib$ mkdir -p build-llvm
~/lib$ bash build-llvm.sh
```

编译Kaleidoscope-LLVM
```bash
~$ mkdir -p build
~$ cd build
~/build$ cmake ..
~/build$ cmake --build .
```

### Architecture

#### `token.h/ast.h`

定义Token和AST

#### `istream.h/lexer.h/parser.h/llvm-codegen.h`

+ `istream.h:IStream`: `file -> Stream`
+ `lexer.h:Lexer`: `Stream -> Generator<Token>`
+ `parser.h:Parser`: `Generator<Token> -> ASTNode`
+ `llvm-codegen.h:LLVMBuilder`: `ASTNode -> llvm::Value`

#### `src/target-llvm/compiler.cpp`

源代码为Kaleidoscope，目标代码为LLVM-IR

### Tutorial

1. `include`中有源码注释。
2. LLVM编译方法参考`lib/build-llvm.sh`和`CMakeLists.txt`。
3. windows下的Mingw64无法编译LLVM，请尝试MSVC工具链。
