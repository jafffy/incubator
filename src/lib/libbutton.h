#ifndef LIBBUTTON_H_
#define LIBBUTTON_H_

struct button_operation
{
	void (*OnButtonClicked)();
	void (*OnButtonPressed)();
	void (*OnButtonReleased)();
};

struct button_node
{
	int x, y;
	int width, height;
	unsigned char* pressed;
	unsigned char* released;

	struct button_operation ops;
};

#endif // LIBBUTTON_H_
