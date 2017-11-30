

module panelLateral_1()
{
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\estructuras\\fanshroud_round2rectangle.stl",convexity = 3);
}

module panelLateral()
{
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\estructuras\\fanshroud_square120.stl",convexity = 3);
}

module frontal()
{
    translate([-72,-32,120])
    rotate([0,0,270])
    panelLateral_1();
    translate([0,120,120])
    rotate([90,180,90])
    panelLateral();
    translate([0,0,120])
    rotate([90,180,90])
    panelLateral();
    translate([-72,32,120])
    rotate([0,0,90])
    mirror([0,1,0])panelLateral_1();
  
}
translate([120,0,0])
rotate([0,0,90])
frontal();