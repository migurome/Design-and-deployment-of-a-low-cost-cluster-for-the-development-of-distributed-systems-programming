
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
    cube([140,90,30]);
}


module box()
{
  
   // translate([155,-240,5])
   // panelLateral();
   // translate([155,-115,5])
   // panelLateral();

    
    color("grey")
    translate([0,-120,3])
    panelLateral();
    color("grey")
    translate([0,-240,3])
    panelLateral();
    color("grey")
    translate([0,-360,3])
    panelLateral();
}

module box_things()
{
    //translate([40,-120,160])
  
    translate([0,-30,3])ventilador();
    translate([120,-30,3])
    ventilador();

   translate([0,-90,30])
    rotate([90,0,90])
    cluster_rasp();
    
    translate([20,-340,5])
    color("RoyalBlue") 
    switch();
}

module caja()
{
    translate([280,-300,0])
    cube([5,300,120]);
    translate([0,-300,0])
    cube([5,300,120]);
    rotate([0,0,90])
    translate([-300,-280,0])
    cube([5,280,120]);
    rotate([0,0,90])
    translate([-5,-280,0])
    cube([5,280,120]);
}


box_things();
caja();




