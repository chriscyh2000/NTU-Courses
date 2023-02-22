from setuptools import find_packages, setup

setup(
    name="tw_rouge",
    packages=find_packages(),
    version="0.1.0",
    description="rouge metric for traditional chinese",
    author="Jonathan Chang",
    license="MIT",
    install_requires=["ckiptagger[tf,gdown]", "rouge"],
)
