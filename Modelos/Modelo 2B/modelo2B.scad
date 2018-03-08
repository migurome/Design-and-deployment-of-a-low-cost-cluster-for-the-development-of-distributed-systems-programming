
module rasp()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelos\\modulos\\rasp.stl",convexity = 3);
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
    
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelos\\modulos\\fan.stl",convexity = 3);
}


module pared()
{
    cube([150,3,150]);
    cylinder(  150,   3, 3,false); 
    cylinder(  150,   3, 3,false);   
}

module switch()
{
    cube([140,90,30]);
}

module pared_tarjetas(){
 
   
 cube([20,5,120]);   
 cube([150,5,20]);
 translate([0,0,40])
 cube([150,5,80]);
 translate([150,0,0])
 cube([115,5,120]);

}


module box()
{
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
    rotate([0,0,90])
    translate([-250,-250,0])
    color("black")
    ladron();
    
    color("red")
    translate([10,-130,150])
    rotate([90,0,90])
    ventilador();
    color("green")
    
    translate([130,-130,150])
    rotate([90,0,90])
    ventilador();

    translate([0,-50,30])
    rotate([90,0,90])
    cluster_rasp();
    
    translate([20,-270,5])
    color("RoyalBlue") 
    switch();
}

module caja()
{
    translate([260,-280,0])
    cube([5,280,120]);
    translate([0,-280,0])
    cube([5,280,120]);
    rotate([0,0,90])
    translate([-280,-260,0])
    cube([5,260,120]);
    pared_tarjetas();
}

module ladron()
{
    cube([300,50,30]);
}


box_things();
caja();



