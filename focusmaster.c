void
focusmaster(const Arg *arg)
{
	Client *c = NULL, *i;

	if (!selmon->sel)
		return;
//	if (arg->i > 0) {
//		for (c = selmon->sel->next; c && !ISVISIBLE(c); c = c->next);
//		if (!c)
//			for (c = selmon->clients; c && !ISVISIBLE(c); c = c->next);
//	} else {
//		for (i = selmon->clients; i != selmon->sel; i = i->next)
//			if (ISVISIBLE(i))
//				c = i;
//		if (!c)
//			for (; i; i = i->next)
//				if (ISVISIBLE(i))
//					c = i;
//	}
	for (c = selmon->clients; !ISVISIBLE(c); c = c->next);
	if (c) {
		focus(c);
		restack(selmon);
	}
}
