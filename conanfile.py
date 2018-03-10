from conans import ConanFile, CMake, tools


class JsondiffcConan(ConanFile):
    name = "jsondiff-c"
    version = "0.1.0"
    license = "MIT"
    url = "https://github.com/franc0is/jsondiff-c"
    description = "A C implementation of Simperium's jsondiff library"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    requires = (("jansson/2.11@franc0is/stable"),
               ("dmp-stl/0.1@franc0is/stable"),
               ("argtable3/3.0.3@franc0is/stable"),
               ("doctest/1.2.6@bincrafters/stable"))

    exports_sources = "*"

    def configure(self):
        self.settings.compiler = 'gcc'
        self.settings.compiler.version = 7
        self.settings.compiler.libcxx = 'libstdc++11'

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".", args=["-DCMAKE_C_COMPILER=gcc-7", "-DCMAKE_CXX_COMPILER=g++-7"])
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src=".")
        self.copy("*jsondiff.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["jsondiff"]
