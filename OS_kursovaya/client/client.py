import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import socket
from threading import Thread
class ClientGUI:
    def __init__(self, master):
        self.master = master
        self.master.title("Казино взломано на 14%")
        self.server_var = tk.StringVar()
        self.server_var.set("127.0.0.1")
        self.create_widgets()
    def create_widgets(self):
        tk.Label(self.master, text="Server IP:").pack(pady=5)
        self.server_entry = tk.Entry(self.master, textvariable=self.server_var)
        self.server_entry.pack(pady=5)
        tk.Button(self.master, text="Connect to Server 1", command=self.connect_to_server_1).pack(pady=5)
        tk.Button(self.master, text="Connect to Server 2", command=self.connect_to_server_2).pack(pady=5)
        tk.Button(self.master, text="Clear Output", command=self.clear_output).pack(pady=5)
        tk.Label(self.master, text="Choose data:").pack(pady=5)
        self.data_choice = ttk.Combobox(self.master, values=["Monitor Info", "Thread & modules Info"])
        self.data_choice.set("Monitor Info")
        self.data_choice.pack(pady=5)
        tk.Button(self.master, text="Get Data", command=self.get_data).pack(pady=5)
        self.output_text = tk.Text(self.master, height=15, width=50)
        self.output_text.pack(pady=5)
    def connect_to_server(self, server_port):
        try:
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((self.server_var.get(), server_port))
            data = client_socket.recv(1024).decode()
            self.output_text.insert(tk.END, f"Received from server: {data}\n")
            client_socket.close()
        except Exception as e:
            messagebox.showerror("Error", f"Connection error: {str(e)}")
    def connect_to_server_1(self):
        self.output_text.insert(tk.END, "Connecting to Server 1...\n")
        Thread(target=self.connect_to_server, args=(8081,)).start()
    def connect_to_server_2(self):
        self.output_text.insert(tk.END, "Connecting to Server 2...\n")
        Thread(target=self.connect_to_server, args=(8082,)).start()
    def clear_output(self):
        self.output_text.delete(1.0, tk.END)
    def get_data(self):
        selected_data = self.data_choice.get()
        if selected_data == "Monitor Info":
            self.connect_to_server(8081)  # Assuming Cursor Info is handled by Server 1
        elif selected_data == "Thread & modules Info":
            self.connect_to_server(8082)  # Assuming Thread Info is handled by Server 2
        else:
            messagebox.showerror("Error", "Invalid data choice.")
if __name__ == "__main__":
    root = tk.Tk()
    root.configure(bg='pink')
    client_gui = ClientGUI(root)
    root.mainloop()

