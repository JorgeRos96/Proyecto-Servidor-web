t <html><head><title>TIME AND DATE</title>
#t <meta http-equiv="refresh" content="1"></meta>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
t var formUpdate = new periodicObj("td.cgx", 1000);
t function plotADGraph() {
t var d= new Date();
t d.setTime((document.getElementById("time").value-3600)*1000);
t var año=d.getFullYear();
t var mes=d.getMonth();
t document.getElementById("time1").value=""+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds();
t document.getElementById("date").value =""+d.getDate()+"/"+mes+"/"+año;
t
#t  alert("Page is loaded");
t }
t function periodicUpdateAd() {
t  if(document.getElementById("adChkBox").checked == true) {
t   updateMultiple(formUpdate,plotADGraph);
t   ad_elTime = setTimeout(periodicUpdateAd, formUpdate.period);
t  }
t  else
t   clearTimeout(ad_elTime);
t }
t </script></head>
t <body onload="periodicUpdateAd()">
i pg_header.inc
t <h2 align="center"><br>Time and Date</h2>
t <p><font size="2">This page allows you to monitor time and date.
t  Periodic screen update is based on <b>xml</b> technology.
t Nota, en la itroduccion manual de fecha y hora, meter hora y
t  fecha en formato XX:XX:XX y XX/XX/XXXX respectivamente, de no hacerlo,
t  no se asegura la correcta actualización de los datos.</font></p>
t <form onLoad="HacerAlgo()" action="datetime.cgi" method="post" name="datetime">
t <input type="hidden" value="datetime" name="pg">
t <table border=0 width=99%><font size="3">
t <tr style="background-color: #aaccff">
t  <th width=50%>Time</th>
t  <th width=50%>Date</th>
t <tr>
t <td align="center"><input type="text" readonly style="background-color: transparent; border: 0px"
t size="10" id="time1" value="%s"></td>
t <td align="center"><input type="text" readonly style="background-color: transparent; border: 0px"
t  size="10" id="date" value="%s"></td>
t </font></table>
t <p align=center>
t <tr><td align=center>EPOCH TIME:
t <input type="text" readonly style="background-color: transparent; border: 0px"
c z 1  size="10" id="time" value="%d"></td></tr>
t </p>
t <p align=center>
t <input type=button style="display: none" value="Refresh" onclick="updateMultiple(formUpdate,plotADGraph)">
t <input style="display: none" checked  type="checkbox" id="adChkBox" onload="periodicUpdateAd()">
t </p>
t <table border=0 width=99%><font size="3">
t <tr style="background-color: #aaccff">
t  <th width=50%>Set Time</th>
t  <th width=50%>Set Date</th>
t </tr><tr>
t <p align=center><tr>
c n 1 <td><input type=text name=tset size=20 maxlength=20 value="%s"></td>
c n 2  <td><input type=text name=dset size=20 maxlength=20 value="%s"></td>
t </tr></p>
t <p align=center><tr>
t <td><input type=submit name=set value="Send" id="sbm"></td></tr></p>
t </font></table>
t </form>
i pg_footer.inc
t </body>
. End of script must be closed with period
