
module rasp()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelos\\modulos\\rasp.stl",convexity = 3);
}

module ladron()
{
    cube([300,50,30]);
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
 
 color("Gainsboro")  
 cube([20,5,120]);   
    color("Gainsboro")
 cube([150,5,20]);
 translate([0,0,40])
    color("Gainsboro")
 cube([150,5,80]);
 translate([150,0,0])
    color("Gainsboro")
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

module flecha(){
    
    
        rotate([0,45,0])

    polyhedron ( points = [[0, -10, 60], [0, 10, 60], [0, 10, 0], [0, -10, 0], [60, -10, 60], [60, 10, 60]], 
triangles = [[0,3,2], [0,2,1], [3,0,4], [1,2,5], [0,5,4], [0,1,5],  [5,2,4], [4,2,3], ]);
    
    

}
module box_things()
{
    rotate([0,0,90])
    translate([-250,-250,0])
    color("IndianRed")
    ladron();
    
    color("LightSlateGray")
    translate([10,-130,150])
    rotate([90,0,90])
    ventilador();
    
    color("LightSlateGray")
    
    translate([130,-130,150])
    rotate([90,0,90])
    ventilador();

    translate([0,-50,30])
    rotate([90,0,90])
    cluster_rasp();
    
    translate([20,-270,5])
    color("SteelBlue") 
    switch();
    
    color("Lime") 
    translate([30,-65,200])
    flecha();
    
    color("red")
    rotate([0,180,0])
    translate([-230,-65,-240])
    flecha();
}

module caja()
{
    
    
    color("Gainsboro")
    translate([260,-280,0])
    cube([5,280,120]);

    //color("Gainsboro")
    //translate([0,-280,0])
    //cube([5,280,120]);

    rotate([0,0,90])
    color("Gainsboro")
    translate([-280,-260,0])
    cube([5,260,120]);
    pared_tarjetas();
    
}

module box(){
box_things();
caja();
}

rotate([0,0,1000* $t])

translate([-120,--120,0])

box();




