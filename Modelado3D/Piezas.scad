
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
}

module cluster_rasp()
{
    module cluster_6()
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
