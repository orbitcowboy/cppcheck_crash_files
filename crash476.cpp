void fun()
{
	([]()
	{
		bool cond;
		if (!cond)
			return;
		const int number;
		switch (number)
		{
			case 1:
			{
				int *pointer;
				if (++(*pointer) < 1)
				{
				}
			}
		}
	});
}
// taken from https://trac.cppcheck.net/ticket/8628
