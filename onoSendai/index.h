const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Power Meter</title>
</head>
<body>
  <h1><center>AYPMABTU?</center></h1>
  <form method="POST" action="/">
    <center><input id ="userInput" name = "userInput" type="text" placeholder="what do you want to say?" required="" pattern=".{3,50}" title="Text must be between 3 and 50 characters"></center>
    <center><input type="Submit" value="SendText"></center>
  </form>
</body>
</html>
)=====";
