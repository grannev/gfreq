import ctypes
import os
import sys
import tkinter as tk
from tkinter import filedialog, messagebox
from pathlib import Path


if sys.platform.startswith("win"):
    lib_name = "gfreq.dll"
elif sys.platform == "darwin":
    lib_name = "./libgfreq.dylib"
else:
    lib_name = "./libgfreq.so"


class gfreq_app:
    def __init__(self):
        self.lib = None
        self.file_path = ""
        self.width = 360
        self.height = 180

    def load_lib(self):
        self.lib = ctypes.CDLL(str(Path(lib_name).resolve()))

        if self.lib is None:
            print("Place dynlib file next to main.py")
            exit(1)

        self.lib.gfreq_pack_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.gfreq_pack_file.restype = ctypes.c_int

        self.lib.gfreq_unpack_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.gfreq_unpack_file.restype = ctypes.c_int

        self.lib.gfreq_strerr.argtypes = [ctypes.c_int]
        self.lib.gfreq_strerr.restype = ctypes.c_char_p

    def init_root(self):
        self.root = tk.Tk()
        self.root.title("gfreq")
        self.root.geometry(f"{self.width}x{self.height}")
        self.root.minsize(self.width, self.height)

        self.label = tk.Label(self.root, text="File not specified", wraplength=320)
        self.label.pack(pady=15)

        tk.Button(self.root, text="Choose file", command=self.choose_file).pack(pady=5)
        tk.Button(self.root, text="Compress / Decompress", command=self.process_file).pack(pady=5)

    def mainloop(self):
        self.fit_window_to_content()
        self.root.mainloop()

    def fit_window_to_content(self):
        self.root.update_idletasks()
        width = max(self.width, self.root.winfo_reqwidth())
        height = max(self.height, self.root.winfo_reqheight())
        self.root.geometry(f"{width}x{height}")


    def choose_file(self):
        self.file_path = filedialog.askopenfilename()
        if self.file_path:
            self.label.config(text=self.file_path)
        else:
            self.label.config(text="File not specified")
        self.fit_window_to_content()


    def process_file(self):
        errstat = 0
        out_file_path = ""

        if self.lib is None:
            return

        if not self.file_path:
            messagebox.showerror("Error", "Choose file")
            return

        if self.file_path.endswith(".ger"):
            out_file_path = self.file_path[:-4]
            errstat = self.lib.gfreq_unpack_file(
                os.fsencode(self.file_path),
                os.fsencode(out_file_path)
            )
            if errstat != 0:
                messagebox.showerror("Error", self.lib.gfreq_strerr(errstat).decode())
                return
            messagebox.showinfo("Done", f"File decompressed:\n{out_file_path}")
            return

        out_file_path = f"{self.file_path}.ger"
        errstat = self.lib.gfreq_pack_file(
            os.fsencode(self.file_path),
            os.fsencode(out_file_path)
        )
        if errstat != 0:
            messagebox.showerror("Error", self.lib.gfreq_strerr(errstat).decode())
            return
        messagebox.showinfo("Done", f"File compressed:\n{out_file_path}")


def main():
    app = gfreq_app()
    app.load_lib()
    app.init_root()
    app.mainloop()
    

if __name__ == "__main__":
    main()
