
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
 
color("grey") 
translate([0,0,135]) 
    rasp();    
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


module ventilador()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelado3D\\modulos\\fan.stl",convexity = 3);
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

module switch()
{
  /* 
    intersection (){
    translate([70,45,0]) 
    resize(newsize=[200,160,70]) sphere(r=10);
    cube([140,90,30]);
        }
        
    */
    cube([140,90,30]);

}


module box()
{
  
   // translate([155,-240,5])
   // panelLateral();
   // translate([155,-115,5])
   // panelLateral();

    //BASE
    translate([0,-300,0])
    minkowski()
    {
      cube([200,300,1.5]);
      //cylinder(r=2,h=1);
      cylinder(2,10,3);
    }
}

module box_things()
{
    translate([0,-55,110])
    rotate([90,180,90])
    usb();

    translate([40,-120,160])
    rotate([90,180,90])
    ventilador();

   translate([0,-50,30])
    rotate([90,0,90])
    cluster_rasp();
    
    translate([40,-280,5])
    color("RoyalBlue") 
    switch();
}

box_things();
box();


