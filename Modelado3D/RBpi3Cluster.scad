module panelLateral()
{
    color("DarkKhaki") 
    translate([0,60,64])
    rotate([0,90,-2])
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\estructuras\\120matrix.stl",convexity = 3);
}

module carcasaVentilador()
{
    color("RoyalBlue") 
    translate([75,-240,65])
    rotate([90,90,0])
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\estructuras\\carcasaventilador.stl",convexity = 3);
}

module rasp()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\rasp.stl",convexity = 3);
}
module cluster_rasp()
{
    
color("YellowGreen") 
translate([0,0,115]) 
    rasp();
color("ForestGreen") 
translate([0,0,95]) 
    rasp();
color("YellowGreen") 
translate([0,0,75]) 
    rasp();
color("ForestGreen") 
translate([0,0,55]) 
    rasp();
color("YellowGreen") 
translate([0,0,35]) 
    rasp();
color("ForestGreen") 
translate([0,0,15]) 
    rasp();
}

module switch()
{
    color("DarkCyan")
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\switch.stl",convexity = 3);
}
module ventilador()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\fan.stl",convexity = 3);
}
module usb3()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\usb3.stl",convexity = 3);
    color("DarkSlateGray")
    translate([0,6,0])
    cube([41,15,128]);
}


module pared()
{
    cube([150,3,150]);
    cylinder(  150,   3, 3,false); 
    cylinder(  150,   3, 3,false);   
}

module liston()
{
    cube([25,3,10]);   
}

module acceso_tarjetas(){
 
    cylinder(  150,   3, 3,false); 
    
    translate([0,25,0])
        cylinder(  150,   3, 3,false); 

    translate([0,0,140])
    rotate([0,0,90])
        liston();

    translate([0,0,0])
    rotate([0,0,90])
        liston();
}

module box()
{
    
    //TARJETAS
    color("RoyalBlue")    translate([0,-55,0])
        acceso_tarjetas();
    translate([155,-240,5])
    panelLateral();
    translate([155,-115,5])
    panelLateral();

    /*
    translate([0,-70,140])
    rotate([0,0,90])
    liston();
    translate([0,-70,0])
    rotate([0,0,90])
    liston();
    rotate([0,0,270])
    translate([0,150,0])
    pared();
    rotate([0,180,90])
    translate([70,0,-150])
    pared();
    */
    //BASE
    translate([0,-240,0])
    minkowski()
    {
      cube([150,240,1.5]);
      //cylinder(r=2,h=1);
      cylinder(2,10,3);
    }
    
    carcasaVentilador();
}
module box_things()
{
    translate([50,-100,10])
    //rotate([90,180,90])
    usb3();

    translate([16,-240,5])
    ventilador();

    translate([50,-40,0])
    rotate([0,0,180])
    cluster_rasp();
}

//box_things();
box();


