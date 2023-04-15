#!/bin/python3

import tkinter as tk
from tkinter import ttk
from tkinter import filedialog as fd

import os

class JknitGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title('JKnit GUI')
        self.root.geometry('300x500')

        self.input = ''
        self.output = ''

        self.doTimer = bool
        self.doLog = bool
        self.doPDF = bool

        self._start_page()
        return
    
    def _start_page(self, message: str=''):
        # Setup
        self._clear()

        tk.Label(text='\nJKnit Graphical User Interface').grid(padx=8)
        tk.Label(text='Jordan Dehmel, 2023-present, GPLv3\n').grid(padx=8)

        # Options checkboxes
        tk.Label(text='Options:').grid()

        self.timerBox = ttk.Checkbutton(self.root,
                    text='Do Timer',
                    variable=self.doTimer,
                    onvalue=True,
                    offvalue=False)
        self.timerBox.grid(sticky=tk.W)

        self.logBox = ttk.Checkbutton(self.root,
                    text='Do Log',
                    variable=self.doLog,
                    onvalue=True,
                    offvalue=False)
        self.logBox.grid(sticky=tk.W)

        self.pdfBox = ttk.Checkbutton(self.root,
                    text='Do PDF Conversion',
                    variable=self.doPDF,
                    onvalue=True,
                    offvalue=False)
        self.pdfBox.grid(sticky=tk.W)

        # Input and output filepaths
        butt = tk.Button(self.root, text='Choose Input File', command=self._get_input_file).grid(sticky=tk.W)
        tk.Button(self.root, text='Choose Output File', command=self._get_output_file).grid(sticky=tk.W)

        # Go button
        tk.Button(self.root, text='Go', command=self._knit).grid(sticky=tk.W)

        tk.Label(self.root, text='\n').grid(sticky=tk.W)

        # Help button
        tk.Button(self.root, text='Help', command=self._help_page).grid()

        # Exit button
        tk.Button(self.root, text='Exit', command=self.root.destroy).grid()

        # Message if there is one
        if message != '':
            tk.Label(self.root, text=message).grid(sticky=tk.W)

        self.root.mainloop()

        return
    
    def _clear(self):
        for child in self.root.winfo_children():
            child.destroy()
        return
    
    def _get_input_file(self):
        self.input = fd.askopenfilename()
        self._start_page('Selected \'' + self.input + '\'')
        return
    
    def _get_output_file(self):
        self.output = fd.asksaveasfilename()
        self._start_page('Selected \'' + self.output + '\'')
        return
    
    def _knit(self):
        if self.input == '':
            self._start_page('ERROR: Please select an input file!')
            return
        elif self.output == '':
            self._start_page('ERROR: Please select an ouput file!')
            return

        self._clear()

        tk.Label(self.root, text='Knitting...').grid(sticky=tk.W)
        tk.Button(self.root, text='Cancel', command=self.root.destroy).grid()

        self.root.update()

        command = 'jknit ' + self.input + ' -o ' + self.output
        
        if self.doLog:
            command += ' -l'
        if self.doTimer:
            command += ' -t'
        
        print('Running command \'' + command + '\'')
        out = os.system(command)
        
        if out == 0:
            print('Command finished with code 0')
        else:
            self._start_page('ERROR: Command exited with code ' + str(out))
            return

        if self.doPDF:
            command = 'pdflatex ' + self.output
            
            print('Running cmmand \'' + command + '\'')
            out = os.system(command)

            if out == 0:
                print('Command finished with code 0')
            else:
                self._start_page('ERROR: Command exited with code ' + str(out))
                return

        self._done_page()
        return

    def _done_page(self):
        self._clear()

        tk.Label(self.root, text='Process completed.').grid(sticky=tk.W)

        tk.Button(self.root, text='Done', command=self.root.destroy).grid()
        return
    
    def _help_page(self):
        self._clear()

        lines: list[str] = ['JKnit is a lightweight, flexible',
                            'open-source mathematical and',
                            'computational document knitter. It',
                            'translates .jmd files (see README.md',
                            'for tips) into LaTeX (.tex) files.',
                            'These .jmd files can have embedded',
                            'running code. You must select an input',
                            'and an output file. "Do Log" creates',
                            'a JKnit log for debugging, "Do Timer"',
                            'times some substeps of the process and',
                            'logs them. "Do PDF" converts the',
                            'output .tex file to a .pdf after',
                            'knitting.\n',
                            'See github.com/jorbDehmel/jknit for',
                            'more info.']

        for line in lines:
            tk.Label(self.root, text=line).grid(sticky=tk.W, pady=3)

        tk.Button(self.root, text='Back', command=self._start_page).grid()

        return


if __name__ == '__main__':
    JknitGUI()
