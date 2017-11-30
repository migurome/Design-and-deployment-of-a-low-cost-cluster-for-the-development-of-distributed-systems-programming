
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

module usb()
{ 
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\usb3.stl",convexity = 3);
    color("DarkSlateGray")
    translate([0,6,0])
    //Medidas reales
    cube([40,25,195]);
    color("Grey") 
    translate([0,0,66])
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\usb3.stl",convexity = 3);
}

module switch()
{
   
    intersection (){
    translate([70,45,0]) 
    resize(newsize=[200,160,70]) sphere(r=10);
    cube([140,90,30]);
        }
}

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

//usb();
color("red")
switch();


