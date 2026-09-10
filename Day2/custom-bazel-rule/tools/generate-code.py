import sys

def main():
    out_path = sys.argv[1]
    message  = sys.argv[2]

    with open(out_path, "w") as f:
        f.write("#pragma once\n")
        f.write("inline const char * getMessage() {\n")
        f.write('\treturn "%s";\n' % message )
        f.write("}\n")

if __name__ == "__main__":
   main()
