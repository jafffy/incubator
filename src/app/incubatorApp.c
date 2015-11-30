
BOOL OnKeyPadPressed(int n)
{

}

struct incubator_operation ops = 
{
	.OnKeyPadPressed = OnKeyPadPressed,
	.OnUpdate		 = OnUpdate,
	.OnDipSwChanged	 = OnDipSwChanged,
};

int main()
{
	incubator_main();

	return 0;
}
