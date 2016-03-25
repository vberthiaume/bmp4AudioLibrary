all:
	#BUILDING STATIC LIBRARY

	#gcc flags:
	#-c: Disables the linker invocation
	#-o: A custom output file
	gcc -c add.c -o add.o
	
	#ar flags:
	#-r: Insert the files member... into archive (with replacement).
	#-c: Indicates you are creating an archive file
	#-s: Write an object-file index into the archive, change is made to the archive
	
	#gcc requires libraries to start with lib and end with .a
	ar rcs libadd.a add.o

	#USING STATIC LIBRARY

	#gcc flags:
	#-L: Indicates the location of the library you want to reference (. = current directory)
	#-l: Specifies the specific library you want to attach
	gcc -c main.c -o main.o
	gcc -c main.exe main.o -L. -ladd
