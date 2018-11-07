
module rasp()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\Modelos\\modulos\\rasp.stl",convexity = 3);
}

module ladron()
{
    cube([250,50,30]);
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
 

translate([150,00,0])
cube([115,5,120]);   
    
translate([0,0,100])
cube([150,5,20]);

cube([150,5,70]);
    
 
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
    rotate([90,0,90])
    translate([-330,0,220])
    color("IndianRed")
    ladron();
    
    color("LightSlateGray")
    translate([10,-130,120])
    rotate([90,0,90])
    ventilador();
    
    color("LightSlateGray")
    
    translate([130,-130,120])
    rotate([90,0,90])
    ventilador();

    translate([170,-50,90])
    rotate([90,180,270])
    cluster_rasp();
    
    translate([10,-340,0])
    color("SteelBlue") 
    switch();
    
    color("Lime") 
    translate([30,-65,160])
    flecha();
    
    color("Lime") 
    translate([150,-65,160])
    flecha();
}

module caja()
{
    
    
    color("Gainsboro")
    translate([260,-350,0])
    cube([5,350,120]);
    

  //  color("Gainsboro")
  //  translate([0,-350,0])
  //  cube([5,350,120]);
    
    rotate([0,0,90])
    color("Gainsboro")
    translate([-350,-260,0])
    cube([5,260,120]);
        color("Gainsboro")

    pared_tarjetas();
    
    color("Gray")
    translate([200,-140,20])
    rotate([0,0,90])
    cube([5,200,100]);
}

module box(){
box_things();
caja();
}

rotate([0,0,1000* $t])
translate([-120,--120,0])
box();




