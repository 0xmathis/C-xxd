#! /usr/bin/env python
import subprocess
from pytest import main, mark, fail
import glob
import sys
import os


BASE_DIR = os.environ.get("BASE_DIR", ".")
BIN_DIR = os.environ.get("BASE_DIR", "bin")
TEST_DIR = os.path.join(BASE_DIR, "tests")
ALL_FILES = glob.glob(os.path.join(TEST_DIR, "*.bin"))
XXD = os.path.join(BASE_DIR, "xxd")


class TestXXD:
    @mark.parametrize("filename", ALL_FILES)
    def test(self, filename):
        expected = subprocess.run(["xxd", filename],
                                  check=True,
                                  capture_output=True,
                                  )
        output = subprocess.run([XXD, filename],
                                check=True,
                                capture_output=True,
                                )

        expected_err = expected.stderr
        expected_out = expected.stdout
        output_err = output.stderr
        output_out = output.stdout

        # if only one raise error
        if expected_err ^ output_err:
            fail(f"""Error fail
                        Expected
                        {expected_err}

                        Output
                        {output_err}""")

        if expected_err and output_err and expected_err != output_err:
            fail(f"""Different error
                        Expected
                        {expected_err}

                        Output
                        {output_err}""")

        expected_split = expected_out.split("b\n")
        output_split = output_out.split("b\n")

        for i in range(max(len(expected_split), len(output_split))):
            try:
                if expected_split[i] != output_split[i]:
                    fail(f"""Error line {i}
                        Expected
                        {expected_split[i]}

                        Output
                        {output_split[i]}""")
            except IndexError:
                # More lines than expected
                if i < len(expected_split):
                    fail(f"Line {i} unexpected\n{output_split[i]}",
                                pytrace=False)

                # Less lines than expected
                if i < len(output_split):
                    fail(f"Line {i} missing\n{expected_split[i]}",
                                pytrace=False)

                fail("Unknown index error",
                            pytrace=False)


main(sys.argv)
