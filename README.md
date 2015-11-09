# Tafels
small program for testing knowledge of time tables

When finished, simply run

$ make
$ sudo make install

to install and

$ sudo make uninstall

to do just that.

It copies the program to ~/home/bin
Make sure the file ~/.profile has the following near the end somewhere

if [ -d "$HOME/bin" ] ; then
  Path="$HOME/bin:$PATH"
fi

See Makefile for further discription
Note, the Makefile was shamelessly copied and adapted from a lecturer, but
I believe it is generic enough that no copyright was enfringed.  

The hashtable is also strongly based on a hashtable of a previous project.  
Once again, a very generic implementation, therefore safe to say not stolen.  
