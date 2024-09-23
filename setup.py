import subprocess

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CustomBuildExt(build_ext):
    def run(self):
        subprocess.check_call(["make", "python_bindings"])
        build_ext.run(self)


hwfp_module = Extension(
    "hwfp._hwfp",
    sources=["src/interface/hwfp.i", "src/cpuinfo.c", "src/boardinfo.c"],
    include_dirs=["src/include"],
    library_dirs=["lib"],
    libraries=["hwfp"],
    swig_opts=["-I./src/include"],
)

setup(
    name="hwfp",
    version="0.1",
    author="AG",
    description="Hardware Fingerprinting Module",
    ext_modules=[hwfp_module],
    py_modules=["hwfp"],
    packages=["hwfp"],
    package_data={"": ["lib/libhwfp.so"]},
    include_package_data=True,
    cmdclass={"build_ext": CustomBuildExt},
)
