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


class GfreqApp:
    def __init__(self):
        self.lib = None
        self.file_path = ""
        self.WIDTH = 360
        self.HEIGHT = 180

    def load_lib(self):
        self.lib = ctypes.CDLL(str(Path(lib_name).resolve()))

        if self.lib is None:
            print("Place dynlib file next to main.py")
            exit(1)

        self.lib.compress.argtypes = [ctypes.c_char_p]
        self.lib.compress.restype = None

        self.lib.decompress.argtypes = [ctypes.c_char_p]
        self.lib.decompress.restype = None

    def init_root(self):
        self.root = tk.Tk()
        self.root.title("gfreq")
        self.root.geometry(f"{self.WIDTH}x{self.HEIGHT}")
        self.root.minsize(self.WIDTH, self.HEIGHT)

        self.label = tk.Label(self.root, text="File not specified", wraplength=320)
        self.label.pack(pady=15)

        tk.Button(self.root, text="Choose file", command=self.choose_file).pack(pady=5)
        tk.Button(self.root, text="Compress / Decompress", command=self.process_file).pack(pady=5)

    def mainloop(self):
        self.fit_window_to_content()
        self.root.mainloop()

    def fit_window_to_content(self):
        self.root.update_idletasks()
        width = max(self.WIDTH, self.root.winfo_reqwidth())
        height = max(self.HEIGHT, self.root.winfo_reqheight())
        self.root.geometry(f"{width}x{height}")


    def choose_file(self):
        self.file_path = filedialog.askopenfilename()
        if self.file_path:
            self.label.config(text=self.file_path)
        else:
            self.label.config(text="File not specified")
        self.fit_window_to_content()


    def process_file(self):
        if self.lib is None:
            return

        if not self.file_path:
            messagebox.showerror("Error", "Choose file")
            return

        if self.file_path.endswith(".ger"):
            self.lib.decompress(os.fsencode(self.file_path))
            messagebox.showinfo("Done", f"File decompressed:\n{self.file_path[:-4]}")
            return

        self.lib.compress(os.fsencode(self.file_path))
        messagebox.showinfo("Done", f"File compressed:\n{self.file_path}.ger")


def main():
    app = GfreqApp()
    app.load_lib()
    app.init_root()
    app.mainloop()
    

if __name__ == "__main__":
    main()

