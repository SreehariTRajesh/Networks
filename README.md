<h3>Toy DNS Implementation</h3>

<ul>
  <li>CLIENT SIDE: Consists of Client Side Code for giving input domain name and receiving response(IP Address)</li>
  <li>LOCAL SERVER: No Cache implemented, yet queries the incoming domain name to the ROOT-SERVER</li>
  <li>ROOT SERVER: Routes the icoming domain name to TLD Server by checking the TLD/Top Level Domain</li>
  <li>TLD SERVER: Routes the incoming domain name to AUTH Server by checking the entire Domain</li>
  <li>AUTH SERVER Contains NS-IPAddr Mapping and returns the IP Address corresponding to the domain</li>
</ul>
