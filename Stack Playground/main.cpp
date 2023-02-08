#include <Windows.h>
#include <iostream>

int TestFunction(int a, int b)
{
	printf("%d", a + b);
	return a + b;
}

// functions create stack frame 
// stack frame are used to store data as function paramaters are stored in stack
// program starts creating stack frame by storing esp in ebp
// push ebp, esp
// then it will subtract an amount of bytes from esp
// this is creating space on the stack for the amount of data needed to be on there
// this space is allocatded between esp and ebp
// ESP points to the top of the stack frame
// EBP points to the bottom of the stack frame
// so logically accessing items in the stack we would add from ebp, and subtract from esp
// access stack items through ebp 
// function params start from ebp+8 as ebp+4 is the function return address
// all stack data is 4 bytes apart from ebp+8

// once function has finished with data stack must be reverted back to original state
// to do this 
/* Test function as assembly*/

int __declspec(naked) TestFunctionAsm(int a, int b)
{
	__asm
	{
		push ebp // ebp is pushed onto the stack to be used to store data
		mov ebp, esp // bottom of stack is stored in ebp by moving esp into ebp
		sub esp, 0xc // 0xc is the size of this data, esp is the top of the stack, so data size is subtrated from it to create space between the top and bottom of the stack for the data going into it 

		mov eax, [ebp + 8] // first parmater of function being moved into eax
		add eax, [ebp + 0xc] // second paramater being added to first paramater stored in eax

		mov esp, ebp // demolishes stack frame
		pop ebp // pop off what was pushed on

		// added to eax as eax is the return register in x86
		ret // ret pops the return address of the function off the stack and jmps to it
	}
}


int main()
{
	__asm
	{
		// FILO - FIRST IN LAST OUT 
		// PUSH - ITEM ADDED TO THE TOP OF THE STACK
		// POP - ITEM REMOVED FROM TOP OF THE STACK
		// The last item added onto the stack will be the first one popped
		/*
		* Stack visual representation 
		* #---#
		* #---#
		* #---#
		* #---#
		* #---#
		* 
		* Stacking like pieces of paper
		*/

		// Stack grows from higher memory addresses to lower addresses
		// goes up to the top1	 to x - the bottom of the stack
		/*
		* #---# 0x0000
		* #---# 0x4
		* #---# 0x8
		* #---# 0xc
		* #---#	0x10
		*/

		mov ax, 16 
		mov bx, 18 
		mov cx, 20 

		push ax // 16 pushed to top of stack
		push bx // 18 pushed to top of stack; 16 now second item on stack
		push cx // 20 now top of stack then 18 then 16

		pop cx // top of stack item into cx; which is currently 20
		pop bx 
		pop ax 
	}

	std::cout << "Raw C++ Function: " << TestFunction(10, 10);;
	std::cout << "Cdecl asm Function: " << TestFunctionAsm(10, 10);
	DWORD garbage;
	std::cin >> garbage;
	return 1;
}