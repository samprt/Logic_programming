#include <tubex.h>
#include <iostream>

using namespace std;
using namespace tubex;

int main()
{
  Interval w(-2, 4);
  cout << "w = " << w << endl;
  Interval z(1, 3);
  cout << "z = " << z << endl;
  cout << "w.z = " << w*z << "\n" << endl;

  Interval a(8, 10);
  cout << "a = " << a << endl;
  Interval l(-1, 0);
  cout << "l = " << l << endl;
  cout << "a/l = " << a/l << "\n" << endl;

  Interval i(-2, 4);
  cout << "i = " << i << endl;
  Interval j(6, 7);
  cout << "j = " << j << endl;
  Interval temp;
  temp = i|j;
  cout << "i|j = " << temp << "\n" << endl;

  Interval r(2, 7);
  cout << "r = " << r << endl;
  Interval s(1, 9);
  cout << "s = " << s << endl;
  cout << "max(r, s) = " << max(r, s) << "\n" << endl;

  Interval e = Interval::EMPTY_SET;
  cout << "e = " << e << endl;
  Interval k(1, 2);
  cout << "k = " << k << endl;
  cout << "max(e, k) = " << max(e, k) << "\n" << endl;

  Interval c;
  cout << "c = " << c << endl;
  cout << "cos(c) = " << cos(c) << "\n" << endl;

	Function g("x[2]", "b[2]", "sqrt((x[0] - b[0])^2 + (x[1] - b[1])^2)");
	IntervalVector x(2, {0, 0});
	IntervalVector b({{3, 4}, {2, 3}});
	Interval d = g.eval(cart_prod(x, b));
	cout << "g([x], [b]) =" << d << "\n" << endl;
  
	vibes::beginDrawing();
	VIBesFigMap fig("Map");
	fig.set_properties(50, 50, 400, 400); // position and size

	fig.draw_box(x);
	fig.draw_box(b);
	fig.draw_circle(0, 0, d.lb());
	fig.draw_circle(0, 0, d.ub());

	x.inflate(0.1);

	d = g.eval(cart_prod(x, b));
	cout << "g([x], [b]) =" << d << "\n" << endl;

	fig.draw_box(x + b, "red[]");
	fig.draw_circle(0, 0, d.lb(), "red");
	fig.draw_circle(0, 0, d.ub(), "red");

	Function temp_f("x[2]", "b[2]", "d", "sqrt((x[0] - b[0])^2 + (x[1] - b[1])^2) - d");
	CtcFunction ctc_dist(temp_f);

	d = Interval(7, 8);
	x = IntervalVector(2, {0, 0});
	IntervalVector b1({{1.5, 2.5}, {4, 11}});
	IntervalVector b2({{3, 4}, {4, 6.5}});
	IntervalVector b3({{5, 7}, {5.5, 8}});

	fig.draw_box(b1);
	fig.draw_box(b2);
	fig.draw_box(b3);
	fig.draw_circle(0, 0, d.lb());
	fig.draw_circle(0, 0, d.ub());

	ContractorNetwork cn;
	cn.add(ctc_dist, {x, b1, d});
	cn.add(ctc_dist, {x, b2, d});
	cn.add(ctc_dist, {x, b3, d});
	cn.contract();

	fig.draw_box(b1, "cyan[cyan]");
	fig.draw_box(b2, "cyan[cyan]");
	fig.draw_box(b3, "cyan[cyan]");
	fig.draw_circle(0, 0, d.lb(), "blue");
	fig.draw_circle(0, 0, d.ub(), "blue");

	fig.show(); // display all items of the figure
	vibes::endDrawing();


// The three domains are then contracted as:
// x=[0, 1], y=[0, 3], a=[1, 4]

  /*
  Tube x(Interval(0,10), 0.01, TFunction("cos(t)+abs(t-5)*[-0.1,0.1]"));

  cout << x << endl;

  vibes::beginDrawing();
  VIBesFigTube fig("My first tube");
  fig.add_tube(&x, "x");
  fig.show();
  vibes::endDrawing();
  */
}