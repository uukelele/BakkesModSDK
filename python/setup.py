from setuptools import setup, Extension
import pybind11
import sys
import os

sdk_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sdk_include = os.path.join(sdk_dir, 'include')
sdk_lib = os.path.join(sdk_dir, 'lib')

# Collect all binding source files
binding_dir = os.path.join(os.path.dirname(__file__), 'bindings')
sources = [
    os.path.join(binding_dir, f)
    for f in os.listdir(binding_dir)
    if f.endswith('.cpp')
]
sources.sort()

ext_modules = [
    Extension(
        'bakkesmod._bindings',
        sources=sources,
        include_dirs=[
            sdk_include,
            pybind11.get_include(),
        ],
        library_dirs=[sdk_lib] if os.path.exists(sdk_lib) else [],
        libraries=['pluginsdk'] if sys.platform == 'win32' else [],
        extra_compile_args=[
            '/std:c++17' if sys.platform == 'win32' else '-std=c++17',
        ],
        extra_link_args=[],
        language='c++',
    ),
]

setup(
    name='bakkesmod',
    version='0.1.0',
    description='CPython bindings for BakkesModSDK - Rocket League plugin SDK',
    long_description=open(os.path.join(os.path.dirname(__file__), '..', 'README.md')).read()
        if os.path.exists(os.path.join(os.path.dirname(__file__), '..', 'README.md'))
        else '',
    long_description_content_type='text/markdown',
    author='BakkesModSDK Contributors',
    url='https://bakkesmod.com/',
    packages=['bakkesmod'],
    package_dir={'bakkesmod': 'bakkesmod'},
    ext_modules=ext_modules,
    zip_safe=False,
    python_requires='>=3.8',
    install_requires=[],
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        'Programming Language :: C++',
        'Topic :: Games/Entertainment',
    ],
)
