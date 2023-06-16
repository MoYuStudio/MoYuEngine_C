.PHONY: all clean

# 定义Raylib所需的变量
PROJECT_NAME       ?= game
RAYLIB_VERSION     ?= 4.2.0
RAYLIB_PATH        ?= ..\..

# 在Windows上定义编译器路径
COMPILER_PATH      ?= C:/raylib/w64devkit/bin

# 定义默认选项
# 平台类型：PLATFORM_DESKTOP（桌面）、PLATFORM_RPI（树莓派）、PLATFORM_ANDROID（安卓）、PLATFORM_WEB（Web）
PLATFORM           ?= PLATFORM_DESKTOP

# Raylib安装的库和头文件路径。在Linux上，如果你安装了Raylib但无法编译示例，请检查这里的*_INSTALL_PATH与src/Makefile中的路径是否相同。
# 若要在编译时启用对libraylib.so的系统范围链接，请运行../src/$ sudo make install RAYLIB_LIBTYPE_SHARED。
# 若要将编译链接到特定版本的libraylib.so，请更改这些变量的值。
# 若要在运行时链接到特定版本的libraylib.so，请参考下面的EXAMPLE_RUNTIME_PATH。
# 如果EXAMPLE_RUNTIME_PATH和RAYLIB_INSTALL_PATH中都有libraylib.so，则运行时EXAMPLE_RUNTIME_PATH中的库（如果存在）将优先于RAYLIB_INSTALL_PATH中的库。
# RAYLIB_INSTALL_PATH应为libraylib的完整路径，不使用相对路径。
DESTDIR ?= /usr/local
RAYLIB_INSTALL_PATH ?= $(DESTDIR)/lib
# RAYLIB_H_INSTALL_PATH定位已安装的raylib头文件和相关源文件。
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include

# Raylib库类型：STATIC（.a）或SHARED（.so/.dll）
RAYLIB_LIBTYPE        ?= STATIC

# 项目的构建模式：DEBUG或RELEASE
BUILD_MODE            ?= RELEASE

# 在Linux桌面上使用外部GLFW库而不是rglfw模块
# TODO: 在Linux上进行评估。选择的目标版本。根据 -lglfw 或 -lglfw3 切换
USE_EXTERNAL_GLFW     ?= FALSE

# 在Linux桌面上使用Wayland显示服务器协议
# 默认使用X11窗口系统
USE_WAYLAND_DISPLAY   ?= FALSE

# 根据选择的PLATFORM_DESKTOP确定PLATFORM_OS
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    # MinGW没有uname.exe！但是在Windows上是OS=Windows_NT！
    # ifeq ($(UNAME),Msys) -> Windows
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
        export PATH := $(COMPILER_PATH):$(PATH)
    else
        UNAMEOS=$(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS=LINUX
        endif
        ifeq ($(UNAMEOS),FreeBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),OpenBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),NetBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),DragonFly)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS=OSX
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    UNAMEOS=$(shell uname)
    ifeq ($(UNAMEOS),Linux)
        PLATFORM_OS=LINUX
    endif
endif

# 根据不同的平台调整RAYLIB_PATH
# 如果使用GNU make，我们可以获取树的顶部的完整路径。Windows？BSD？
# 对于ldconfig或其他不执行路径扩展的工具是必需的。
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        RAYLIB_PREFIX ?= ..
        RAYLIB_PATH    = $(realpath $(RAYLIB_PREFIX))
    endif
endif
# Raspberry Pi上的Raylib默认路径，如果安装在不同路径，请更新它！
# 目前src/Makefile未使用该路径。不确定其起源或用法。请参考wiki。
# TODO：更新src/Makefile中的install：目标，考虑与LINUX的关系。
ifeq ($(PLATFORM),PLATFORM_RPI)
    RAYLIB_PATH       ?= /home/pi/raylib
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
    # Emscripten所需变量
    EMSDK_PATH         ?= C:/emsdk
    EMSCRIPTEN_PATH    ?= $(EMSDK_PATH)/upstream/emscripten
    CLANG_PATH          = $(EMSDK_PATH)/upstream/bin
    PYTHON_PATH         = $(EMSDK_PATH)/python/3.9.2-1_64bit
    NODE_PATH           = $(EMSDK_PATH)/node/14.18.2_64bit/bin
    export PATH         = $(EMSDK_PATH);$(EMSCRIPTEN_PATH);$(CLANG_PATH);$(NODE_PATH);$(PYTHON_PATH):$$(PATH)
endif

# 定义raylib编译后的发布目录。
# RAYLIB_RELEASE_PATH指向提供的二进制文件或新构建版本
RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src

# EXAMPLE_RUNTIME_PATH将自定义运行时库（如libraylib.so）的位置嵌入到使用RAYLIB_LIBTYPE=SHARED编译的每个示例二进制文件中。
# 默认为RAYLIB_RELEASE_PATH，以便这些示例在链接时与../release/libs/linux中的libraylib.so链接，
# 而无需从../src/Makefile进行正式安装。它有助于可移植性，并且对于具有多个raylib版本、在非标准位置安装raylib或希望将libraylib.so与游戏捆绑在一起的情况非常有用。
# 将其更改为所需的路径。
# 注意：如果在运行时EXAMPLE_RUNTIME_PATH和RAYLIB_INSTALL_PATH中都有libraylib.so，
# 则存在EXAMPLE_RUNTIME_PATH中的库将优先于RAYLIB_INSTALL_PATH中的库。
EXAMPLE_RUNTIME_PATH   ?= $(RAYLIB_RELEASE_PATH)

# 定义默认的C编译器：gcc
# 注意：如果使用C++，请定义g++编译器
CC = gcc

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),OSX)
        # OSX上的默认编译器
        CC = clang
    endif
    ifeq ($(PLATFORM_OS),BSD)
        # FreeBSD、OpenBSD、NetBSD、DragonFly上的默认编译器
        CC = clang
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    ifeq ($(USE_RPI_CROSS_COMPILER),TRUE)
        # 定义RPI交叉编译器
        #CC = armv6j-hardfloat-linux-gnueabi-gcc
        CC = $(RPI_TOOLCHAIN)/bin/arm-linux-gnueabihf-gcc
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # HTML5 emscripten编译器
    # 警告：要编译为HTML5，代码必须重新设计以使用emscripten.h和emscripten_set_main_loop()
    CC = emcc
endif

# 定义编译器选项：
#  -O0                  定义优化级别（无优化，适用于调试）
#  -O1                  定义优化级别
#  -g                   包含调试信息
#  -s                   从构建中删除不必要的数据 -> 不要在调试构建中使用
#  -Wall                打开大部分编译器警告
#  -std=c99             定义C语言模式（C标准从1999版）
#  -std=gnu99           定义C语言模式（GNU C标准从1999版）
#  -Wno-missing-braces  忽略无效警告（GCC bug 53119）
#  -D_DEFAULT_SOURCE    在Linux和PLATFORM_WEB上使用 -std=c99 时使用，需要timespec
CFLAGS += -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
else
    CFLAGS += -s -O1
endif

# 额外的编译器选项（如果需要）
#CFLAGS += -Wextra -Wmissing-prototypes -Wstrict-prototypes
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        # 资源文件包含Windows可执行文件的图标和属性
        # -Wl,--subsystem,windows隐藏控制台窗口
        CFLAGS += $(RAYLIB_PATH)/src/raylib.rc.data -Wl,--subsystem,windows
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),STATIC)
            CFLAGS += -D_DEFAULT_SOURCE
        endif
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            # 显式启用对libraylib.so的运行时链接
            CFLAGS += -Wl,-rpath,$(EXAMPLE_RUNTIME_PATH)
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    CFLAGS += -std=gnu99
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # -Os                        # 大小优化
    # -O2                        # 优化级别2，如果使用了，也设置 --memory-init-file 0
    # -s USE_GLFW=3              # 使用glfw3库（上下文/输入管理）
    # -s ALLOW_MEMORY_GROWTH=1   # 允许内存调整大小 -> 警告：音频缓冲可能失败！
    # -s TOTAL_MEMORY=16777216   # 指定堆内存大小（默认为16MB）
    # -s USE_PTHREADS=1          # 多线程支持
    # -s WASM=0                  # 禁用Web Assembly，默认会生成
    # -s EMTERPRETIFY=1          # 启用emscripten代码解释器（非常慢）
    # -s EMTERPRETIFY_ASYNC=1    # 支持emterpreter的同步循环
    # -s FORCE_FILESYSTEM=1      # 强制文件系统加载/保存文件数据
    # -s ASSERTIONS=1            # 在常见内存分配错误的运行时检查（-O1及以上版本会关闭）
    # --profiling                # 包括代码分析信息
    # --memory-init-file 0       # 避免外部内存初始化代码文件（.mem）
    # --preload-file resources   # 指定资源文件夹用于数据编译
    CFLAGS += -Os -s USE_GLFW=3 -s TOTAL_MEMORY=16777216 --preload-file resources
    ifeq ($(BUILD_MODE), DEBUG)
        CFLAGS += -s ASSERTIONS=1 --profiling
    endif

    # 定义自定义shell .html和输出扩展名
    CFLAGS += --shell-file $(RAYLIB_PATH)/src/shell.html
    EXT = .html
endif

# 定义所需的头文件的包含路径
# 注意：多个外部所需库（如stb等）
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external

# 定义包含所需头文件的其他目录
ifeq ($(PLATFORM),PLATFORM_RPI)
    # RPI所需库
    INCLUDE_PATHS += -I/opt/vc/include
    INCLUDE_PATHS += -I/opt/vc/include/interface/vmcs_host/linux
    INCLUDE_PATHS += -I/opt/vc/include/interface/vcos/pthreads
endif
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),BSD)
        # 考虑 -L$(RAYLIB_H_INSTALL_PATH)
        INCLUDE_PATHS += -I/usr/local/include
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        # 重置所有内容。
        # 优先级：当前目录，安装版本，raysan5提供的库 -I$(RAYLIB_H_INSTALL_PATH) -I$(RAYLIB_PATH)/release/include
        INCLUDE_PATHS = -I$(RAYLIB_H_INSTALL_PATH) -isystem. -isystem$(RAYLIB_PATH)/src -isystem$(RAYLIB_PATH)/release/include -isystem$(RAYLIB_PATH)/src/external
    endif
endif

# 定义包含所需库的库路径
LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),BSD)
        # 考虑 -L$(RAYLIB_INSTALL_PATH)
        LDFLAGS += -L. -Lsrc -L/usr/local/lib
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),STATIC)
            LDFLAGS += -L$(RAYLIB_PATH)/src
        endif
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDFLAGS += -L$(EXAMPLE_RUNTIME_PATH)
        endif
    endif
endif

# 需要链接的库
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),STATIC)
            LDLIBS = -lraylib -lglfw -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
        endif
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDLIBS = -lraylib -lglfw -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
        endif
        ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
            # 需要安装libwayland-dev和libxkbcommon-dev
            LDLIBS += -lwayland-client -lwayland-cursor -lxkbcommon
        endif
    endif
    ifeq ($(PLATFORM_OS),OSX)
        LDLIBS = -lraylib -framework OpenGL -framework IOKit -framework Cocoa -framework CoreVideo
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    LDLIBS = -lraylib -lbrcmGLESv2 -lbrcmEGL -lpthread -lrt -lm -lbcm_host -L/opt/vc/lib
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    LDLIBS = -lraylib -lglfw3 -lm
endif

# 定义需要编译的源文件
SOURCE_FILES = main.c

# 定义生成的可执行文件的名称
BUILD_FILENAME = $(PROJECT_NAME)

# 定义生成的可执行文件的输出目录
BUILD_OUTPUT_PATH = .

# 定义最终生成的可执行文件的完整路径
BUILD_FULL_PATH = $(BUILD_OUTPUT_PATH)/$(BUILD_FILENAME)$(EXT)

# 默认目标
all: $(BUILD_FULL_PATH)

# 编译生成可执行文件
$(BUILD_FULL_PATH): $(SOURCE_FILES)
	$(CC) $(SOURCE_FILES) -o $(BUILD_FULL_PATH) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) $(CFLAGS)

# 清除生成的文件
clean:
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
		del *.o *.exe /s
    endif
    ifeq ($(PLATFORM_OS),LINUX)
	find -type f -executable | xargs file -i | grep -E 'x-object|x-archive|x-sharedlib|x-executable' | rev | cut -d ':' -f 2- | rev | xargs rm -fv
    endif
    ifeq ($(PLATFORM_OS),OSX)
		find . -type f -perm +ugo+x -delete
		rm -f *.o
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
	find . -type f -executable -delete
	rm -fv *.o
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	del *.o *.html *.js
endif
	@echo Cleaning done

