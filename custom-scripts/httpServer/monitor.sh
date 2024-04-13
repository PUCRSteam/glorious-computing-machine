#! /bin/sh

cat > index.html << EOF

<html>
	<head>
		<title>HTML System Monitor - T1 Lab Sisop</title>
	</head>
	<body>
		<h1>Bash System Monitor</h1>
		<h2>Trabalho 1 - Laboratorio de sistemas operacionais</h2>
		<h3>Guilherme Vieira e Marco Ames</h3>

		<p>Date: $(date)</p>
		<p>Uptime: $(cat /proc/uptime | awk '{print $1}') Seconds</p>
		<p>CPU model name: $(awk -F: '/model name/ {print $2; exit}' /proc/cpuinfo)</p>
		<p>Current Core(s) Frequency: $(awk '/cpu MHz/ {print $4 " MHz"}' /proc/cpuinfo)</p>
		<p>CPU Utilization: $(top -n 1 | grep "CPU:" | awk 'NR==1 {print $2 + $4 " %"}')</p>
		<p>RAM memory consumption:</p>
		<p>Total: $(free -h | grep Mem: | awk '{print $2}')</p>
		<p>Used: $(free -h | grep Mem: | awk '{print $3 " (" $3/$2*100 "%)"}')</p>
		<p>Free: $(free -h | grep Mem: | awk '{print $4 " (" $4/$2*100 "%)"}')</p>
		<p>System version: $(uname -a)</p>
		<p>List of running processes: </p>
		$(ps | awk '{print $1 "\t" $3 "<br>"}')
	</body>
</html>

EOF
