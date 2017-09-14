void room()   /****** LOOP: RUNS CONSTANTLY ******/
{
  lightLevel_R =analogRead(photoPin_R);
  lightLevel_L =analogRead(photoPin_L);
  lightLevel_up =analogRead(photoPin_up);
  
 //Send the adjusted Light level result to Serial port (processing)
 /*
  Serial.print("L: ");
  Serial.print(lightLevel_L);
  Serial.print(" - up: ");
  Serial.print(lightLevel_up);
  Serial.print(" - R: ");
  Serial.println(lightLevel_R);
 */
 
 //within 15 of each other
 if((lightLevel_R - lightLevel_L < 31 and lightLevel_R - lightLevel_L > -1) or (lightLevel_L - lightLevel_R < 31 and lightLevel_L - lightLevel_R > -1)) {
  /*Serial.print("L: ");
  Serial.print(lightLevel_L);
  Serial.print(" - R: ");
  Serial.println(lightLevel_R);*/
     slowForward();
     delay(250);
   //}
 }
 
 //Right PhotoCell is picking up significantly more.
 else if(lightLevel_R - lightLevel_L > 30) {
     turnRight90();
     delay(250);
   //}
 }
 
 //Left PhotoCell is picking up significantly more.
 else if(lightLevel_L - lightLevel_R > 30) {
     turnLeft90();
     delay(250);
   //}
}

 else {
  Serial.println("idk");
 }
  
  
}//--(end main loop )---
