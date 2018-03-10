from conans import ConanFile, CMake, tools
import os

class JsondiffcTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def configure(self):
        self.settings.compiler = 'gcc'
        self.settings.compiler.version = 7
        self.settings.compiler.libcxx = 'libstdc++11'

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".", args=["-DCMAKE_C_COMPILER=gcc-7", "-DCMAKE_CXX_COMPILER=g++-7"])
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        if not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run(".%sexample" % os.sep)
