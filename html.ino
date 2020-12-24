#define APP_NAME "IoT Heater"

// Head partials
// Style
const String styles = ""
  "<style>"
    "body{}"
    "nav a:not(:last-child) {margin-right: 0.5em;}"
    "footer {margin-top: 1em;}"
  "</style>";

// Body partials
const String header = ""
  "<header>"
    "<h1>"+String(APP_NAME)+"</h1>"
  "</header>";

const String nav = ""
  "<nav class='nav'>"
    "<a href='/'>Home</a>"
    "<a href='/update'>Firmware update</a>"
  "</nav>";
  
const String footer= ""
  "<footer>"
    "<div>"+String(APP_NAME)+"</div>"
    "<div>Maxime Moreillon</div>"
  "</footer>";

const String firmware_update_subheader = ""
  "<h2>Firmware update</h2>";
  
// Update form
String update_form = ""
  +firmware_update_subheader+
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Update'>"
  "</form>";

// Combinations
String head = ""
  "<head>"
    "<title>"+String(APP_NAME)+" | " + String(HOSTNAME) +"</title>"
    + styles +
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "</head>";

String pre_main = ""
  "<html>"
    + head + 
    "<body>"
      + header + nav +
      "<main>";
  
String post_main = ""
      "</main>"
      + footer +
    "</body>"
  "</html>";

// Root
String root_main(){

  //time_t current_time = DateTime.now();
  
  return ""
  "<h2>Home</h2>"
  "<p>Hostname: " + String(HOSTNAME) + "</p>"
  "<p>Firmware: v" + String(FIRMWARE_VERSION) + "</p>"
  "<p>"
    "Uptime: " +uptime_formatter::getUptime()+ ""
  "</p>"
  "<p><a href='/on'>Turn on</a></p>"
  "<p><a href='/off'>Turn off</a></p>";
}




// Not found
String not_found = "<h2>Not found</h2>";
