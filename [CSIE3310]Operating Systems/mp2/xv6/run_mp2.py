#!/usr/bin/env python

import re
from gradelib import *


@test(0, "mp2_1")
def test_mp2_1():
    r = Runner(save("mp2_1.out"))
    r.run_qemu(shell_script(["mp2_1"]), timeout=300)


@test(0, "mp2_2")
def test_mp2_2():
    r = Runner(save("mp2_2.out"))
    r.run_qemu(shell_script(["mp2_2"]), timeout=300)


@test(0, "mp2_3")
def test_mp2_3():
    r = Runner(save("mp2_3.out"))
    r.run_qemu(shell_script(["mp2_3"]), timeout=300)


@test(0, "mp2_4")
def test_mp2_4():
    r = Runner(save("mp2_4.out"))
    r.run_qemu(shell_script(["mp2_4"]), timeout=300)


run_tests()
