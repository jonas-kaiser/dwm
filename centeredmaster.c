void
centeredmaster(Monitor *m)
{
	unsigned int i, n, h, mw, mx, my, oty, ety, tw;
	float mfacts = 0, lfacts = 0, rfacts = 0;
	Client *c;

	/* count number of clients in the selected monitor */
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else if ((n - m->nmaster) % 2)
			lfacts += c->cfact;
		else
			rfacts += c->cfact;
	}
	if (n == 0)
		return;
	if (n == 1) {
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	/* initialize areas */
	mw = m->ww;
	mx = 0;
	my = 0;
	tw = mw;

	if (n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		mw = m->nmaster ? m->ww * m->mfact : 0;
		tw = m->ww - mw;

		if (n - m->nmaster > 1) {
			/* only one client */
			mx = (m->ww - mw) / 2;
			tw = (m->ww - mw) / 2;
		}
	}

	oty = 0;
	ety = 0;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	if (i < m->nmaster) {
		/* nmaster clients are stacked vertically, in the center
		 * of the screen */
		h = (m->wh - my) * (c->cfact / mfacts);
		resize(c, m->wx + mx, m->wy + my, mw - (2*c->bw),
		       h - (2*c->bw), 0);
		my += HEIGHT(c);
		mfacts -= c->cfact;
	} else {
		/* stack clients are stacked vertically */
		if ((i - m->nmaster) % 2) {
			h = (m->wh - ety) * (c->cfact / lfacts);
			//TODO don't need if
			if(m->nmaster == 0)
				resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
				       h - (2*c->bw), 0);
			else
				resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
				       h - (2*c->bw), 0);
			ety += HEIGHT(c);
			lfacts -= c->cfact;
		} else {
			h = (m->wh - oty) * (c->cfact / rfacts);
			resize(c, m->wx + mx + mw, m->wy + oty,
			       tw - (2*c->bw), h - (2*c->bw), 0);
			oty += HEIGHT(c);
			rfacts -= c->cfact;
		}
	}
}

void
centeredfloatingmaster(Monitor *m)
{
	unsigned int i, n, w, mh, mw, mx, mxo, my, myo, tx;
	Client *c;

	/* count number of clients in the selected monitor */
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	/* initialize nmaster area */
	if (n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh) {
			mw = m->nmaster ? m->ww * m->mfact : 0;
			mh = m->nmaster ? m->wh * 0.9 : 0;
		} else {
			mh = m->nmaster ? m->wh * m->mfact : 0;
			mw = m->nmaster ? m->ww * 0.9 : 0;
		}
		mx = mxo = (m->ww - mw) / 2;
		my = myo = (m->wh - mh) / 2;
	} else {
		/* go fullscreen if all clients are in the master area */
		mh = m->wh;
		mw = m->ww;
		mx = mxo = 0;
		my = myo = 0;
	}

	for(i = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	if (i < m->nmaster) {
		/* nmaster clients are stacked horizontally, in the center
		 * of the screen */
		w = (mw + mxo - mx) / (MIN(n, m->nmaster) - i);
		resize(c, m->wx + mx, m->wy + my, w - (2*c->bw),
		       mh - (2*c->bw), 0);
		mx += WIDTH(c);
	} else {
		/* stack clients are stacked horizontally */
		w = (m->ww - tx) / (n - i);
		resize(c, m->wx + tx, m->wy, w - (2*c->bw),
		       m->wh - (2*c->bw), 0);
		tx += WIDTH(c);
	}
}
