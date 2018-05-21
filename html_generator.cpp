//
// Created by Girts Rudziss on 14/05/2018.
//
#include "html_generator.h"

namespace pt=boost::property_tree;
// Quick and dirty method of generating HTML (see example HTML and Javascript files for normal syntax)
void generate_report(pt::ptree &root, std::string project) {
  std::stringstream json;
  pt::json_parser::write_json(json, root);
  std::ofstream output(project + "/" + project + ".html");
  output << "<!DOCTYPE html>\n"
      "<html lang=\"en\">\n"
      "<head>\n"
      "    <meta charset=\"UTF-8\">\n"
      "    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>\n"
      "    <script>\n"
      "  $(document).ready(function() {\n"
      "       var json = " << json.rdbuf() << ";\n"
      "  var info = document.getElementById(\"info\");\n"
      "  var title = document.getElementById((\"title\"));\n"
      "  title.innerHTML = \"Analyzer tool\";\n"
      "  info.appendChild(document.createTextNode(\"Project \" + json[\"project\"] + \" analysis results\"));\n"
      "  var totals = document.getElementById(\"totals\");\n"
      "  totals.innerHTML = \"<ul><li>Version: \" + json[\"version\"] + \"</li>\" +\n"
      "    \"<li>Total size: \" + (json['totalsize'] / 1024).toFixed(2) + \" kilobytes </li>\" +\n"
      "    \"<li>Total source lines of code: \" + json[\"totalsloc\"] + \"</li>\" +\n"
      "    \"<li>Total comment lines of code: \" + json[\"totalcloc\"] + \"</li></ul>\";\n"
      "  var history = json[\"history\"];\n"
      "  add_history(history);\n"
      "  var breakdown = document.getElementById(\"breakdown\");\n"
      "  var breakdown_list = document.createElement(\"ul\");\n"
      "  breakdown.appendChild(breakdown_list);\n"
      "  iterate(json, breakdown_list);\n"
      "});\n"
      "\n"
      "function iterate(object, list) {\n"
      "  console.log(object);\n"
      "\n"
      "  var files = object[\"files\"];\n"
      "  for (var filekey in files) {\n"
      "    var file = files[filekey];\n"
      "    append_file(file, list);\n"
      "  }\n"
      "\n"
      "  var subdirectories = object[\"subdirectories\"];\n"
      "  for (var dirkey in subdirectories) {\n"
      "    var directory = subdirectories[dirkey];\n"
      "    append_subdir(directory, list);\n"
      "  }\n"
      "};\n"
      "\n"
      "function append_file(file, list) {\n"
      "  console.log(file);\n"
      "  var element = document.createElement(\"li\");\n"
      "  element.innerHTML = \"File: \" + file.filename;   //Add filename\n"
      "  $(element).hover(function() {\n"
      "    $(this).css('cursor','pointer');          //Set cursor to pointer on hover\n"
      "  }, function() {\n"
      "    $(this).css('cursor','auto');\n"
      "  });\n"
      "  list.appendChild(element);\n"
      "\n"
      "  var stats = document.createElement(\"ul\");\n"
      "  if (file.source_loc !== undefined) {\n"
      "    stats.innerHTML = \"Size: \" + file.size +\n"
      "      \" bytes; Source LOC: \" + file.source_loc +\n"
      "      \"; Comment LOC: \" + file.comment_loc;\n"
      "  } else {\n"
      "    stats.innerHTML = \"Size: \" + file.size + \" bytes\";\n"
      "  }\n"
      "  element.appendChild(stats);\n"
      "\n"
      "  $(stats).hide();       //Hide stats by default\n"
      "  $(element).click(function() {\n"
      "    $(stats).toggle(300);   //Show or hide file stats when filename is clicked\n"
      "  });\n"
      "};\n"
      "\n"
      "function append_subdir(directory, list) {\n"
      "  var element = document.createElement(\"li\");\n"
      "  element.innerHTML = \"<strong>Subdirectory: \" + directory[\"directory\"] + \"</strong> (Size of files in this directory: \" + directory[\"size\"] + \" bytes)\";\n"
      "  $(element).hover(function() {\n"
      "    $(this).css('cursor','pointer');    //Set cursor to pointer on hover\n"
      "  }, function() {\n"
      "    $(this).css('cursor','auto');\n"
      "  });\n"
      "  list.appendChild(element);\n"
      "  var sublist_div = document.createElement(\"div\");\n"
      "  var sublist = document.createElement(\"ul\");       //New list for files and subdirs in this directory\n"
      "  list.appendChild(sublist_div);\n"
      "  sublist_div.appendChild(sublist);\n"
      "  iterate(directory, sublist);\n"
      "  $(sublist_div).children().hide();       //Hide subdirs by default\n"
      "  $(element).click(function() {\n"
      "    $(sublist_div).children().toggle(500); //Show or hide file stats when direcory name is clicked\n"
      "  });\n"
      "};\n"
      "\n"
      "function add_history(history) {\n"
      "  console.log(history);\n"
      "\n"
      "  var info = document.getElementById(\"graph_info\");\n"
      "  var list = document.createElement(\"ol\");\n"
      "\n"
      "  var canvas = document.getElementById(\"canvas\");\n"
      "  canvas.width = history.length * 100;    //Each bar is 100px\n"
      "  canvas.height = 250;                    //Canvas height = 250px\n"
      "  var myChart = new Chart(\n"
      "    {\n"
      "      canvas: canvas,\n"
      "      data: history,\n"
      "      colors: [\"#a55ca5\", \"#67b6c7\", \"#bccd7a\", \"#eb9743\"]\n"
      "    }\n"
      "  );\n"
      "  myChart.draw();\n"
      "};\n"
      "\n"
      "function fillBackground(ctx, width, height) {\n"
      "  ctx.save();\n"
      "  ctx.fillStyle = \"#eeeeee\";\n"
      "  ctx.fillRect(0, 0, width, height);\n"
      "}\n"
      "\n"
      "function drawBar(ctx, upperLeftCornerX, upperLeftCornerY, width, height, value, version, color) {\n"
      "  ctx.save();\n"
      "  ctx.fillStyle = color;\n"
      "  ctx.fillRect(upperLeftCornerX, upperLeftCornerY, width, height);   //Fill bar with a color\n"
      "\n"
      "  ctx.fillStyle = \"#000000\";      //Add black text for size\n"
      "  ctx.font = \"10px Arial\";\n"
      "  var text = (value / 1024).toFixed(2) + \" kilobytes\";    //Convert value to kilobytes\n"
      "  var textWidth = ctx.measureText(text).width;\n"
      "  var posX = upperLeftCornerX + ((width - textWidth) / 2);    //Text X coodrdinate (centered above the bar)\n"
      "  var posY = canvas.height - height - 40;  //Place the text above the bar\n"
      "  ctx.textBaseline = \"top\";\n"
      "  ctx.fillText(text, posX, posY);\n"
      "\n"
      "  var version_width = ctx.measureText(version).width;\n"
      "  posX = upperLeftCornerX + ((width - version_width) / 2);\n"
      "  posY = canvas.height - 15;\n"
      "  ctx.fillText(version, posX, posY);\n"
      "  ctx.restore();\n"
      "}\n"
      "\n"
      "var Chart = function (options) {\n"
      "  this.options = options;\n"
      "  this.canvas = options.canvas;\n"
      "  this.ctx = this.canvas.getContext(\"2d\");\n"
      "  this.colors = options.colors;\n"
      "\n"
      "  this.draw = function () {\n"
      "    var maxValue = 0;\n"
      "    for (var element in this.options.data) {\n"
      "      var curValue = parseInt(this.options.data[element].size);\n"
      "      if (curValue > maxValue) {\n"
      "        maxValue = curValue;\n"
      "      }\n"
      "    }\n"
      "    var minValue = maxValue;\n"
      "    for (var element in this.options.data) {\n"
      "      var curValue = parseInt(this.options.data[element].size);\n"
      "      if (curValue < minValue) {\n"
      "        minValue = curValue;\n"
      "      }\n"
      "    }\n"
      "\n"
      "    //drawing the bars\n"
      "    var barIndex = 0;\n"
      "    var numberOfBars = this.options.data.length;\n"
      "    var barWidth = (this.canvas.width / numberOfBars);\n"
      "\n"
      "    var bar_area_height = this.canvas.height - 20;  //20 off the bottom to add version\n"
      "    var minHeight = bar_area_height * 0.1;     //Minimum height of a bar (10% of max area height)\n"
      "    var maxHeight = bar_area_height - 30;     //Maximum height of a bar (-30px for size text above the bar)\n"
      "\n"
      "    fillBackground(this.ctx, this.canvas.width, bar_area_height);     //Fill bar area with a grey background\n"
      "    for (element in this.options.data) {\n"
      "      var value = this.options.data[element].size;\n"
      "      var version = this.options.data[element].version;\n"
      "\n"
      "      // Height relative to max and min values\n"
      "      if (minValue === maxValue) var barHeight = maxHeight;\n"
      "      else var barHeight = minHeight + ((maxHeight - minHeight) * (value - minValue) / (maxValue - minValue));\n"
      "\n"
      "      drawBar(\n"
      "        this.ctx,\n"
      "        barIndex * barWidth,            //Upper left corner X\n"
      "        bar_area_height - barHeight, //Upper left corner Y\n"
      "        barWidth,\n"
      "        barHeight,\n"
      "        value,\n"
      "        version,\n"
      "        this.colors[barIndex % this.colors.length]\n"
      "      );\n"
      "      barIndex++;\n"
      "    }\n"
      "  }\n"
      "};"
      "</script>\n"
      "    <title id=\"title\">Title</title>\n"
      "</head>\n"
      "<body>\n"
      "    <div id=\"content\" style=\"padding-bottom: 10%; width:100vw\">\n"
      "        <h2 id=\"info\"></h2>\n"
      "        <div id=\"totals\"> </div>\n"
      "        <h3> Version size history </h3>\n"
      "        <p id=\"graph_info\"></p>\n"
      "        <div id=\"graph\" style=\"overflow:scroll;\">\n"
      "            <canvas id=\"canvas\"></canvas>\n"
      "        </div>\n"
      "        <h4> Project file breakdown </h4>\n"
      "        <div id=\"breakdown\"> </div>\n"
      "    </div>\n"
      "</body>\n"
      "</html>";
}
