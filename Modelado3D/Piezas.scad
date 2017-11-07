
module ventilador()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\fan.stl",convexity = 3);
}

module rasp()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\rasp.stl",convexity = 3);
}

module usb1()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\usb3.stl",convexity = 3);
}

module usb2()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\usb2.stl",convexity = 3);
}

module usb3()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\usb3.stl",convexity = 3);
    color("DarkSlateGray")
    translate([0,6,0])
    cube([41,15,128]);
}

module switch()
{
    color("DarkCyan")
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\switch.stl",convexity = 3);
}

switch();

module cluster_rasp()
{
translate([0,0,115]) 
    rasp();
translate([0,0,95]) 
    rasp();
translate([0,0,75]) 
    rasp();
translate([0,0,55]) 
    rasp();
translate([0,0,35]) 
    rasp();
translate([0,0,15]) 
    rasp();
}


module switch()
{
    color("DarkCyan")
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\switch.stl",convexity = 3);
}
