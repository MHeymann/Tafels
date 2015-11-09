# Tafels
small program for testing knowledge of time tables

# Installation
When finished, simply run

	$ make
	$ sudo make install

to install and

	$ sudo make uninstall

to do just that.

# Details on how it installs
It copies the program to ~/home/bin
Make sure the file ~/.profile has the following near the end somewhere

	if [ -d "$HOME/bin" ] ; then
		Path="$HOME/bin:$PATH"
	fi

See Makefile for further discription.

# Copyright
Note, the Makefile was shamelessly copied and adapted from a lecturer, but
I believe it is generic enough that no copyright was enfringed.  If 
this is not the case, feel free to contact me to educate
	
	heymann.murray@gmail.com

The hashtable is also strongly based on a hashtable of a previous project.  
Once again, a very generic implementation, therefore not stolen.  
Also, this is a personal project, not meant for commercial use.  Not
Supposed to be used to show my own work per se, but an attempt to create
something I enjoy.  
