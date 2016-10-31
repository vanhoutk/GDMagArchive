const int visualBaseNumVerts = 535;
const int visualBaseNumIndices = 264*3;

float visualBaseNorms[535*3];
unsigned char visualBaseColrs[535*4];
float visualBaseVerts[535*3] = {
-2.175227, 0.637105, 0.001539,
-2.267873, 0.052160, 0.001539,
-2.205967, -0.536832, 0.001539,
-1.993729, -1.089733, 0.001539,
-1.645621, -1.568862, 0.001539,
-1.185366, -1.941568, 0.001539,
-0.644331, -2.182453, 0.001539,
-0.059386, -2.275099, 0.001539,
0.529606, -2.213193, 0.001539,
1.082507, -2.000955, 0.001539,
1.561636, -1.652847, 0.001539,
1.934343, -1.192593, 0.001539,
2.175227, -0.651557, 0.001539,
2.267873, -0.066612, 0.001539,
2.205967, 0.522380, 0.001539,
1.993729, 1.075280, 0.001539,
1.645620, 1.554410, 0.001539,
1.185366, 1.927117, 0.001539,
0.644331, 2.168001, 0.001539,
0.059386, 2.260647, 0.001539,
-0.529606, 2.198741, 0.001539,
-1.082507, 1.986502, 0.001539,
-1.561636, 1.638394, 0.001539,
-1.934343, 1.178140, 0.001539,
-2.175227, 0.637105, 0.435110,
-2.267873, 0.052160, 0.435110,
-2.205967, -0.536832, 0.435110,
-1.993729, -1.089733, 0.435110,
-1.645621, -1.568862, 0.435110,
-1.185366, -1.941568, 0.435110,
-0.644331, -2.182453, 0.435110,
-0.059386, -2.275099, 0.435110,
0.529606, -2.213193, 0.435110,
1.082507, -2.000955, 0.435110,
1.561636, -1.652847, 0.435110,
1.934343, -1.192593, 0.435110,
2.175227, -0.651557, 0.435110,
2.267873, -0.066612, 0.435110,
2.205967, 0.522380, 0.435110,
1.993729, 1.075280, 0.435110,
1.645620, 1.554410, 0.435110,
1.185366, 1.927117, 0.435110,
0.644331, 2.168001, 0.435110,
0.059386, 2.260647, 0.435110,
-0.529606, 2.198741, 0.435110,
-1.082507, 1.986502, 0.435110,
-1.561636, 1.638394, 0.435110,
-1.934343, 1.178140, 0.435110,
-1.266968, 0.025951, 0.435110,
-1.215210, 0.352735, 0.435110,
-1.232384, -0.303095, 0.435110,
-1.113815, -0.611978, 0.435110,
-0.919341, -0.879648, 0.435110,
-0.662216, -1.087864, 0.435110,
-0.359962, -1.222436, 0.435110,
-0.033177, -1.274194, 0.435110,
0.295869, -1.239610, 0.435110,
0.604752, -1.121041, 0.435110,
0.872422, -0.926567, 0.435110,
1.080638, -0.669442, 0.435110,
1.215211, -0.367188, 0.435110,
1.266968, -0.040403, 0.435110,
1.232384, 0.288643, 0.435110,
1.113815, 0.597526, 0.435110,
0.919341, 0.865196, 0.435110,
0.662216, 1.073412, 0.435110,
0.359961, 1.207984, 0.435110,
0.033176, 1.259742, 0.435110,
-0.295869, 1.225158, 0.435110,
-0.604752, 1.106589, 0.435110,
-0.872422, 0.912115, 0.435110,
-1.080638, 0.654989, 0.435110,
-0.000000, -0.007226, 0.089419,
-1.029839, 0.297826, 0.089419,
-1.073702, 0.020890, 0.089419,
-1.044393, -0.257963, 0.089419,
-0.943911, -0.519728, 0.089419,
-0.779103, -0.746567, 0.089419,
-0.561200, -0.923021, 0.089419,
-0.305052, -1.037066, 0.089419,
-0.028116, -1.080928, 0.089419,
0.250737, -1.051619, 0.089419,
0.512502, -0.951137, 0.089419,
0.739341, -0.786329, 0.089419,
0.915795, -0.568426, 0.089419,
1.029839, -0.312278, 0.089419,
1.073702, -0.035342, 0.089419,
1.044393, 0.243510, 0.089419,
0.943911, 0.505276, 0.089419,
0.779103, 0.732115, 0.089419,
0.561200, 0.908569, 0.089419,
0.305052, 1.022613, 0.089419,
0.028116, 1.066475, 0.089419,
-0.250737, 1.037167, 0.089419,
-0.512502, 0.936685, 0.089419,
-0.739341, 0.771876, 0.089419,
-0.915795, 0.553974, 0.089419,
-0.872422, 0.912115, 0.204305,
-1.008422, 0.744169, 0.204305,
-1.080638, 0.654989, 0.204305,
-1.127312, 0.550158, 0.204305,
-1.215210, 0.352735, 0.204305,
-1.254973, -0.088173, 0.204305,
-1.266968, 0.025951, 0.204305,
-1.232384, -0.303095, 0.204305,
-1.233162, 0.239396, 0.204305,
-1.046365, -0.704815, 0.204305,
-1.113815, -0.611978, 0.204305,
-0.919341, -0.879648, 0.204305,
-1.191260, -0.410226, 0.204305,
-0.557384, -1.134538, 0.204305,
-0.662216, -1.087864, 0.204305,
-0.359962, -1.222436, 0.204305,
-0.830162, -0.951864, 0.204305,
0.080947, -1.262199, 0.204305,
-0.033177, -1.274194, 0.204305,
0.295869, -1.239610, 0.204305,
-0.246622, -1.240388, 0.204305,
0.697589, -1.053591, 0.204305,
0.604752, -1.121041, 0.204305,
0.872422, -0.926567, 0.204305,
0.403000, -1.198486, 0.204305,
1.127312, -0.564610, 0.204305,
1.080638, -0.669442, 0.204305,
1.215211, -0.367188, 0.204305,
0.944638, -0.837388, 0.204305,
1.254973, 0.073721, 0.204305,
1.266968, -0.040403, 0.204305,
1.232384, 0.288643, 0.204305,
1.233162, -0.253848, 0.204305,
1.046365, 0.690363, 0.204305,
1.113815, 0.597526, 0.204305,
0.919341, 0.865196, 0.204305,
1.191260, 0.395774, 0.204305,
0.557384, 1.120086, 0.204305,
0.662216, 1.073412, 0.204305,
0.359961, 1.207984, 0.204305,
0.830162, 0.937412, 0.204305,
-0.080947, 1.247747, 0.204305,
0.033176, 1.259742, 0.204305,
-0.295869, 1.225158, 0.204305,
0.246622, 1.225935, 0.204305,
-0.697589, 1.039139, 0.204305,
-0.604752, 1.106589, 0.204305,
-0.403000, 1.184034, 0.204305,
-2.267873, 0.052160, 0.001539,
-2.267873, 0.052160, 0.435110,
-2.267873, 0.052160, 0.001539,
-2.205967, -0.536832, 0.001539,
-2.205967, -0.536832, 0.435110,
-2.205967, -0.536832, 0.001539,
-1.993729, -1.089733, 0.001539,
-1.993729, -1.089733, 0.435110,
-1.993729, -1.089733, 0.001539,
-1.645621, -1.568862, 0.001539,
-1.645621, -1.568862, 0.435110,
-1.645621, -1.568862, 0.001539,
-1.185366, -1.941568, 0.001539,
-1.185366, -1.941568, 0.435110,
-1.185366, -1.941568, 0.001539,
-0.644331, -2.182453, 0.001539,
-0.644331, -2.182453, 0.435110,
-0.644331, -2.182453, 0.001539,
-0.059386, -2.275099, 0.001539,
-0.059386, -2.275099, 0.435110,
-0.059386, -2.275099, 0.001539,
0.529606, -2.213193, 0.001539,
0.529606, -2.213193, 0.435110,
0.529606, -2.213193, 0.001539,
1.082507, -2.000955, 0.001539,
1.082507, -2.000955, 0.435110,
1.082507, -2.000955, 0.001539,
1.561636, -1.652847, 0.001539,
1.561636, -1.652847, 0.435110,
1.561636, -1.652847, 0.001539,
1.934343, -1.192593, 0.001539,
1.934343, -1.192593, 0.435110,
1.934343, -1.192593, 0.001539,
2.175227, -0.651557, 0.001539,
2.175227, -0.651557, 0.435110,
2.175227, -0.651557, 0.001539,
2.267873, -0.066612, 0.001539,
2.267873, -0.066612, 0.435110,
2.267873, -0.066612, 0.001539,
2.205967, 0.522380, 0.001539,
2.205967, 0.522380, 0.435110,
2.205967, 0.522380, 0.001539,
1.993729, 1.075280, 0.001539,
1.993729, 1.075280, 0.435110,
1.993729, 1.075280, 0.001539,
1.645620, 1.554410, 0.001539,
1.645620, 1.554410, 0.435110,
1.645620, 1.554410, 0.001539,
1.185366, 1.927117, 0.001539,
1.185366, 1.927117, 0.435110,
1.185366, 1.927117, 0.001539,
0.644331, 2.168001, 0.001539,
0.644331, 2.168001, 0.435110,
0.644331, 2.168001, 0.001539,
0.059386, 2.260647, 0.001539,
0.059386, 2.260647, 0.435110,
0.059386, 2.260647, 0.001539,
-0.529606, 2.198741, 0.001539,
-0.529606, 2.198741, 0.435110,
-0.529606, 2.198741, 0.001539,
-1.082507, 1.986502, 0.001539,
-1.082507, 1.986502, 0.435110,
-1.082507, 1.986502, 0.001539,
-1.561636, 1.638394, 0.001539,
-1.561636, 1.638394, 0.435110,
-1.561636, 1.638394, 0.001539,
-1.934343, 1.178140, 0.001539,
-2.175227, 0.637105, 0.435110,
-1.934343, 1.178140, 0.435110,
-1.934343, 1.178140, 0.001539,
-2.175227, 0.637105, 0.001539,
-2.175227, 0.637105, 0.435110,
-2.175227, 0.637105, 0.435110,
-2.267873, 0.052160, 0.435110,
-1.934343, 1.178140, 0.435110,
-1.934343, 1.178140, 0.435110,
-1.934343, 1.178140, 0.435110,
-2.267873, 0.052160, 0.435110,
-1.645621, -1.568862, 0.435110,
-1.993729, -1.089733, 0.435110,
-1.645621, -1.568862, 0.435110,
-1.185366, -1.941568, 0.435110,
-2.205967, -0.536832, 0.435110,
-1.993729, -1.089733, 0.435110,
-2.267873, 0.052160, 0.435110,
-1.993729, -1.089733, 0.435110,
-2.267873, 0.052160, 0.435110,
-1.185366, -1.941568, 0.435110,
2.267873, -0.066612, 0.435110,
2.205967, 0.522380, 0.435110,
2.175227, -0.651557, 0.435110,
2.175227, -0.651557, 0.435110,
1.934343, -1.192593, 0.435110,
2.175227, -0.651557, 0.435110,
2.175227, -0.651557, 0.435110,
2.205967, 0.522380, 0.435110,
1.993729, 1.075280, 0.435110,
2.205967, 0.522380, 0.435110,
-1.934343, 1.178140, 0.435110,
-1.561636, 1.638394, 0.435110,
-1.934343, 1.178140, 0.435110,
-1.185366, -1.941568, 0.435110,
-0.644331, -2.182453, 0.435110,
-0.644331, -2.182453, 0.435110,
-0.644331, -2.182453, 0.435110,
-0.059386, -2.275099, 0.435110,
-0.059386, -2.275099, 0.435110,
-0.059386, -2.275099, 0.435110,
0.529606, -2.213193, 0.435110,
0.529606, -2.213193, 0.435110,
0.529606, -2.213193, 0.435110,
1.082507, -2.000955, 0.435110,
1.082507, -2.000955, 0.435110,
1.082507, -2.000955, 0.435110,
1.561636, -1.652847, 0.435110,
1.561636, -1.652847, 0.435110,
1.561636, -1.652847, 0.435110,
1.934343, -1.192593, 0.435110,
1.934343, -1.192593, 0.435110,
1.934343, -1.192593, 0.435110,
1.934343, -1.192593, 0.435110,
1.934343, -1.192593, 0.435110,
1.993729, 1.075280, 0.435110,
1.645620, 1.554410, 0.435110,
1.645620, 1.554410, 0.435110,
1.645620, 1.554410, 0.435110,
1.185366, 1.927117, 0.435110,
1.185366, 1.927117, 0.435110,
1.185366, 1.927117, 0.435110,
0.644331, 2.168001, 0.435110,
0.644331, 2.168001, 0.435110,
0.644331, 2.168001, 0.435110,
0.059386, 2.260647, 0.435110,
0.059386, 2.260647, 0.435110,
0.059386, 2.260647, 0.435110,
-0.529606, 2.198741, 0.435110,
-0.529606, 2.198741, 0.435110,
-0.529606, 2.198741, 0.435110,
-1.082507, 1.986502, 0.435110,
-1.082507, 1.986502, 0.435110,
-1.082507, 1.986502, 0.435110,
-1.561636, 1.638394, 0.435110,
-1.561636, 1.638394, 0.435110,
-1.561636, 1.638394, 0.435110,
-1.561636, 1.638394, 0.435110,
-1.561636, 1.638394, 0.435110,
-1.934343, 1.178140, 0.435110,
-0.872422, 0.912115, 0.435110,
-1.008422, 0.744169, 0.204305,
-1.215210, 0.352735, 0.435110,
-1.127312, 0.550158, 0.204305,
-1.254973, -0.088173, 0.204305,
-1.232384, -0.303095, 0.435110,
-1.215210, 0.352735, 0.204305,
-1.266968, 0.025951, 0.204305,
-1.233162, 0.239396, 0.204305,
-1.266968, 0.025951, 0.435110,
-1.046365, -0.704815, 0.204305,
-0.919341, -0.879648, 0.435110,
-1.232384, -0.303095, 0.204305,
-1.113815, -0.611978, 0.204305,
-1.191260, -0.410226, 0.204305,
-1.113815, -0.611978, 0.435110,
-0.557384, -1.134538, 0.204305,
-0.359962, -1.222436, 0.435110,
-0.919341, -0.879648, 0.204305,
-0.662216, -1.087864, 0.204305,
-0.830162, -0.951864, 0.204305,
-0.662216, -1.087864, 0.435110,
0.080947, -1.262199, 0.204305,
0.295869, -1.239610, 0.435110,
-0.359962, -1.222436, 0.204305,
-0.033177, -1.274194, 0.204305,
-0.246622, -1.240388, 0.204305,
-0.033177, -1.274194, 0.435110,
0.697589, -1.053591, 0.204305,
0.872422, -0.926567, 0.435110,
0.295869, -1.239610, 0.204305,
0.604752, -1.121041, 0.204305,
0.403000, -1.198486, 0.204305,
0.604752, -1.121041, 0.435110,
1.127312, -0.564610, 0.204305,
1.215211, -0.367188, 0.435110,
0.872422, -0.926567, 0.204305,
1.080638, -0.669442, 0.204305,
0.944638, -0.837388, 0.204305,
1.080638, -0.669442, 0.435110,
1.254973, 0.073721, 0.204305,
1.232384, 0.288643, 0.435110,
1.215211, -0.367188, 0.204305,
1.266968, -0.040403, 0.204305,
1.233162, -0.253848, 0.204305,
1.266968, -0.040403, 0.435110,
1.046365, 0.690363, 0.204305,
0.919341, 0.865196, 0.435110,
1.232384, 0.288643, 0.204305,
1.113815, 0.597526, 0.204305,
1.191260, 0.395774, 0.204305,
1.113815, 0.597526, 0.435110,
0.557384, 1.120086, 0.204305,
0.359961, 1.207984, 0.435110,
0.919341, 0.865196, 0.204305,
0.662216, 1.073412, 0.204305,
0.830162, 0.937412, 0.204305,
0.662216, 1.073412, 0.435110,
-0.080947, 1.247747, 0.204305,
-0.295869, 1.225158, 0.435110,
0.359961, 1.207984, 0.204305,
0.033176, 1.259742, 0.204305,
0.246622, 1.225935, 0.204305,
0.033176, 1.259742, 0.435110,
-0.872422, 0.912115, 0.435110,
-0.872422, 0.912115, 0.204305,
-0.697589, 1.039139, 0.204305,
-0.295869, 1.225158, 0.204305,
-0.604752, 1.106589, 0.204305,
-0.403000, 1.184034, 0.204305,
-0.604752, 1.106589, 0.435110,
-0.872422, 0.912115, 0.204305,
-1.008422, 0.744169, 0.204305,
-1.008422, 0.744169, 0.204305,
-1.080638, 0.654989, 0.435110,
-1.080638, 0.654989, 0.204305,
-1.008422, 0.744169, 0.204305,
-1.080638, 0.654989, 0.435110,
-1.008422, 0.744169, 0.204305,
-0.872422, 0.912115, 0.435110,
-1.127312, 0.550158, 0.204305,
-1.215210, 0.352735, 0.204305,
-1.215210, 0.352735, 0.204305,
-1.215210, 0.352735, 0.435110,
-1.127312, 0.550158, 0.204305,
-1.080638, 0.654989, 0.204305,
-1.215210, 0.352735, 0.435110,
-1.080638, 0.654989, 0.204305,
-1.080638, 0.654989, 0.435110,
-1.266968, 0.025951, 0.204305,
-1.232384, -0.303095, 0.435110,
-1.254973, -0.088173, 0.204305,
-1.266968, 0.025951, 0.435110,
-1.232384, -0.303095, 0.435110,
-1.266968, 0.025951, 0.204305,
-1.232384, -0.303095, 0.204305,
-1.232384, -0.303095, 0.204305,
-1.215210, 0.352735, 0.204305,
-1.266968, 0.025951, 0.435110,
-1.233162, 0.239396, 0.204305,
-1.215210, 0.352735, 0.435110,
-1.266968, 0.025951, 0.435110,
-1.215210, 0.352735, 0.204305,
-1.113815, -0.611978, 0.204305,
-0.919341, -0.879648, 0.435110,
-1.046365, -0.704815, 0.204305,
-1.113815, -0.611978, 0.435110,
-0.919341, -0.879648, 0.435110,
-1.113815, -0.611978, 0.204305,
-0.919341, -0.879648, 0.204305,
-0.919341, -0.879648, 0.204305,
-1.232384, -0.303095, 0.204305,
-1.113815, -0.611978, 0.435110,
-1.191260, -0.410226, 0.204305,
-1.232384, -0.303095, 0.435110,
-1.113815, -0.611978, 0.435110,
-1.232384, -0.303095, 0.204305,
-0.662216, -1.087864, 0.204305,
-0.359962, -1.222436, 0.435110,
-0.557384, -1.134538, 0.204305,
-0.662216, -1.087864, 0.435110,
-0.359962, -1.222436, 0.435110,
-0.662216, -1.087864, 0.204305,
-0.359962, -1.222436, 0.204305,
-0.359962, -1.222436, 0.204305,
-0.919341, -0.879648, 0.204305,
-0.662216, -1.087864, 0.435110,
-0.830162, -0.951864, 0.204305,
-0.919341, -0.879648, 0.435110,
-0.662216, -1.087864, 0.435110,
-0.919341, -0.879648, 0.204305,
-0.033177, -1.274194, 0.204305,
0.295869, -1.239610, 0.435110,
0.080947, -1.262199, 0.204305,
-0.033177, -1.274194, 0.435110,
0.295869, -1.239610, 0.435110,
-0.033177, -1.274194, 0.204305,
0.295869, -1.239610, 0.204305,
0.295869, -1.239610, 0.204305,
-0.359962, -1.222436, 0.204305,
-0.033177, -1.274194, 0.435110,
-0.246622, -1.240388, 0.204305,
-0.359962, -1.222436, 0.435110,
-0.033177, -1.274194, 0.435110,
-0.359962, -1.222436, 0.204305,
0.604752, -1.121041, 0.204305,
0.872422, -0.926567, 0.435110,
0.697589, -1.053591, 0.204305,
0.604752, -1.121041, 0.435110,
0.872422, -0.926567, 0.435110,
0.604752, -1.121041, 0.204305,
0.872422, -0.926567, 0.204305,
0.872422, -0.926567, 0.204305,
0.295869, -1.239610, 0.204305,
0.604752, -1.121041, 0.435110,
0.403000, -1.198486, 0.204305,
0.295869, -1.239610, 0.435110,
0.604752, -1.121041, 0.435110,
0.295869, -1.239610, 0.204305,
1.080638, -0.669442, 0.204305,
1.215211, -0.367188, 0.435110,
1.127312, -0.564610, 0.204305,
1.080638, -0.669442, 0.435110,
1.215211, -0.367188, 0.435110,
1.080638, -0.669442, 0.204305,
1.215211, -0.367188, 0.204305,
1.215211, -0.367188, 0.204305,
0.872422, -0.926567, 0.204305,
1.080638, -0.669442, 0.435110,
0.944638, -0.837388, 0.204305,
0.872422, -0.926567, 0.435110,
1.080638, -0.669442, 0.435110,
0.872422, -0.926567, 0.204305,
1.266968, -0.040403, 0.204305,
1.232384, 0.288643, 0.435110,
1.254973, 0.073721, 0.204305,
1.266968, -0.040403, 0.435110,
1.232384, 0.288643, 0.435110,
1.266968, -0.040403, 0.204305,
1.232384, 0.288643, 0.204305,
1.232384, 0.288643, 0.204305,
1.215211, -0.367188, 0.204305,
1.266968, -0.040403, 0.435110,
1.233162, -0.253848, 0.204305,
1.215211, -0.367188, 0.435110,
1.266968, -0.040403, 0.435110,
1.215211, -0.367188, 0.204305,
1.113815, 0.597526, 0.204305,
0.919341, 0.865196, 0.435110,
1.046365, 0.690363, 0.204305,
1.113815, 0.597526, 0.435110,
0.919341, 0.865196, 0.435110,
1.113815, 0.597526, 0.204305,
0.919341, 0.865196, 0.204305,
0.919341, 0.865196, 0.204305,
1.232384, 0.288643, 0.204305,
1.113815, 0.597526, 0.435110,
1.191260, 0.395774, 0.204305,
1.232384, 0.288643, 0.435110,
1.113815, 0.597526, 0.435110,
1.232384, 0.288643, 0.204305,
0.662216, 1.073412, 0.204305,
0.359961, 1.207984, 0.435110,
0.557384, 1.120086, 0.204305,
0.662216, 1.073412, 0.435110,
0.359961, 1.207984, 0.435110,
0.662216, 1.073412, 0.204305,
0.359961, 1.207984, 0.204305,
0.359961, 1.207984, 0.204305,
0.919341, 0.865196, 0.204305,
0.662216, 1.073412, 0.435110,
0.830162, 0.937412, 0.204305,
0.919341, 0.865196, 0.435110,
0.662216, 1.073412, 0.435110,
0.919341, 0.865196, 0.204305,
0.033176, 1.259742, 0.204305,
-0.295869, 1.225158, 0.435110,
-0.080947, 1.247747, 0.204305,
0.033176, 1.259742, 0.435110,
-0.295869, 1.225158, 0.435110,
0.033176, 1.259742, 0.204305,
-0.295869, 1.225158, 0.204305,
-0.295869, 1.225158, 0.204305,
0.359961, 1.207984, 0.204305,
0.033176, 1.259742, 0.435110,
0.246622, 1.225935, 0.204305,
0.359961, 1.207984, 0.435110,
0.033176, 1.259742, 0.435110,
0.359961, 1.207984, 0.204305,
-0.697589, 1.039139, 0.204305,
-0.872422, 0.912115, 0.204305,
-0.872422, 0.912115, 0.204305,
-0.872422, 0.912115, 0.435110,
-0.604752, 1.106589, 0.204305,
-0.872422, 0.912115, 0.435110,
-0.604752, 1.106589, 0.204305,
-0.604752, 1.106589, 0.435110,
-0.295869, 1.225158, 0.204305,
-0.604752, 1.106589, 0.435110,
-0.403000, 1.184034, 0.204305,
-0.295869, 1.225158, 0.435110,
-0.604752, 1.106589, 0.435110,
-0.295869, 1.225158, 0.204305 };

float visualBaseTexCoords[535*2] = {
0.000000, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.117821, 0.876812,
0.000000, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.117821, 1.000000,
0.770506, 0.571515,
0.779921, 0.500000,
0.742902, 0.638157,
0.698990, 0.695384,
0.641763, 0.739296,
0.575122, 0.766899,
0.503606, 0.776315,
0.432091, 0.766900,
0.365449, 0.739296,
0.308222, 0.695384,
0.264310, 0.638157,
0.236706, 0.571516,
0.227291, 0.500000,
0.236706, 0.428484,
0.264310, 0.361843,
0.308222, 0.304616,
0.365449, 0.260704,
0.432090, 0.233100,
0.503606, 0.223685,
0.575122, 0.233100,
0.641763, 0.260704,
0.698990, 0.304616,
0.742902, 0.361843,
0.770506, 0.428484,
0.503606, 0.500000,
0.737771, 0.500000,
0.729792, 0.560606,
0.706399, 0.617082,
0.669186, 0.665580,
0.620689, 0.702793,
0.564212, 0.726186,
0.503606, 0.734165,
0.443000, 0.726186,
0.386523, 0.702793,
0.338026, 0.665580,
0.300813, 0.617083,
0.277420, 0.560606,
0.269441, 0.500000,
0.277420, 0.439394,
0.300813, 0.382918,
0.338026, 0.334420,
0.386523, 0.297207,
0.443000, 0.273814,
0.503606, 0.265835,
0.564212, 0.273814,
0.620688, 0.297207,
0.669186, 0.334420,
0.706399, 0.382917,
0.729792, 0.439394,
0.422254, 0.601931,
0.595564, 0.601931,
0.770506, 0.428484,
0.422254, 0.428620,
0.422254, 0.601931,
0.760932, 0.594629,
0.770506, 0.571515,
0.422254, 0.601931,
0.776655, 0.524804,
0.679142, 0.710614,
0.698990, 0.695384,
0.422254, 0.601931,
0.727672, 0.658005,
0.550318, 0.770165,
0.575122, 0.766899,
0.422254, 0.601931,
0.618650, 0.748869,
0.408977, 0.757326,
0.432091, 0.766900,
0.422254, 0.601931,
0.478802, 0.773049,
0.292992, 0.675536,
0.308222, 0.695384,
0.422254, 0.601931,
0.345601, 0.724066,
0.233441, 0.546712,
0.236706, 0.571516,
0.422254, 0.601931,
0.254736, 0.615044,
0.246280, 0.405371,
0.236706, 0.428485,
0.422254, 0.601931,
0.230557, 0.475196,
0.328070, 0.289386,
0.308222, 0.304616,
0.422254, 0.601931,
0.279540, 0.341995,
0.456894, 0.229835,
0.432090, 0.233100,
0.422254, 0.601931,
0.388562, 0.251131,
0.598235, 0.242674,
0.575122, 0.233100,
0.422254, 0.601931,
0.528410, 0.226951,
0.595564, 0.601931,
0.698990, 0.304616,
0.661611, 0.275934,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.000000, 1.000000,
0.000000, 0.876812,
0.000000, 0.876812,
0.117821, 1.000000,
0.000000, 1.000000,
0.000000, 0.876812,
0.117821, 0.876812,
0.117821, 1.000000,
0.998209, 0.500000,
0.981356, 0.628013,
0.981356, 0.371987,
0.981356, 0.371987,
0.981356, 0.371987,
0.981356, 0.628013,
0.750908, 0.928339,
0.853343, 0.849737,
0.750908, 0.928339,
0.631619, 0.977750,
0.931945, 0.747302,
0.853343, 0.849737,
0.981356, 0.628013,
0.853343, 0.849737,
0.981356, 0.628013,
0.631619, 0.977750,
0.025856, 0.371987,
0.075267, 0.252698,
0.009003, 0.500000,
0.009003, 0.500000,
0.025856, 0.628013,
0.009003, 0.500000,
0.009003, 0.500000,
0.075267, 0.252698,
0.153868, 0.150263,
0.075267, 0.252698,
0.981356, 0.371987,
0.931945, 0.252698,
0.981356, 0.371987,
0.631619, 0.977750,
0.503606, 0.994603,
0.503606, 0.994603,
0.503606, 0.994603,
0.375593, 0.977750,
0.375593, 0.977750,
0.375593, 0.977750,
0.256304, 0.928339,
0.256304, 0.928339,
0.256304, 0.928339,
0.153869, 0.849738,
0.153869, 0.849738,
0.153869, 0.849738,
0.075267, 0.747302,
0.075267, 0.747302,
0.075267, 0.747302,
0.025856, 0.628013,
0.025856, 0.628013,
0.025856, 0.628013,
0.025856, 0.628013,
0.025856, 0.628013,
0.153868, 0.150263,
0.256304, 0.071661,
0.256304, 0.071661,
0.256304, 0.071661,
0.375593, 0.022250,
0.375593, 0.022250,
0.375593, 0.022250,
0.503606, 0.005397,
0.503606, 0.005397,
0.503606, 0.005397,
0.631619, 0.022250,
0.631619, 0.022250,
0.631619, 0.022250,
0.750908, 0.071661,
0.750908, 0.071661,
0.750908, 0.071661,
0.853343, 0.150263,
0.853343, 0.150263,
0.853343, 0.150263,
0.931945, 0.252698,
0.931945, 0.252698,
0.931945, 0.252698,
0.931945, 0.252698,
0.931945, 0.252698,
0.981356, 0.371987,
0.422254, 0.428620,
0.760932, 0.405371,
0.595564, 0.601931,
0.773771, 0.453288,
0.422254, 0.428620,
0.595564, 0.601931,
0.779921, 0.500000,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.742902, 0.638157,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.641763, 0.739296,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.503606, 0.776315,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.365449, 0.739296,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.264310, 0.638157,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.227291, 0.500000,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.264310, 0.361843,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.365449, 0.260704,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.503606, 0.223685,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.422254, 0.428620,
0.595564, 0.428620,
0.714220, 0.324464,
0.641763, 0.260704,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.742902, 0.361843,
0.760932, 0.405371,
0.760932, 0.405371,
0.595564, 0.601931,
0.422254, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.773771, 0.453288,
0.779921, 0.500000,
0.779921, 0.500000,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.742902, 0.638157,
0.742902, 0.638157,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.422254, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.641763, 0.739296,
0.641763, 0.739296,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.503606, 0.776315,
0.503606, 0.776315,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.365449, 0.739296,
0.365449, 0.739296,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.264310, 0.638157,
0.264310, 0.638157,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.227291, 0.500000,
0.227291, 0.500000,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.264310, 0.361843,
0.264310, 0.361843,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.365449, 0.260704,
0.365449, 0.260704,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.503606, 0.223685,
0.503606, 0.223685,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.641763, 0.260704,
0.641763, 0.260704,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620,
0.714220, 0.324464,
0.742902, 0.361843,
0.742902, 0.361843,
0.422254, 0.428620,
0.422254, 0.601931,
0.595564, 0.601931,
0.422254, 0.428620,
0.595564, 0.428620,
0.422254, 0.428620,
0.595564, 0.601931,
0.422254, 0.601931,
0.595564, 0.428620,
0.595564, 0.601931,
0.422254, 0.428620 };

unsigned int visualBaseIndices[264*3] = {
24, 25, 0, 
25, 1, 0, 
146, 26, 145, 
26, 2, 147, 
149, 27, 148, 
27, 3, 150, 
152, 28, 151, 
28, 4, 153, 
155, 29, 154, 
29, 5, 156, 
158, 30, 157, 
30, 6, 159, 
161, 31, 160, 
31, 7, 162, 
164, 32, 163, 
32, 8, 165, 
167, 33, 166, 
33, 9, 168, 
170, 34, 169, 
34, 10, 171, 
173, 35, 172, 
35, 11, 174, 
176, 36, 175, 
36, 12, 177, 
179, 37, 178, 
37, 13, 180, 
182, 38, 181, 
38, 14, 183, 
185, 39, 184, 
39, 15, 186, 
188, 40, 187, 
40, 16, 189, 
191, 41, 190, 
41, 17, 192, 
194, 42, 193, 
42, 18, 195, 
197, 43, 196, 
43, 19, 198, 
200, 44, 199, 
44, 20, 201, 
203, 45, 202, 
45, 21, 204, 
206, 46, 205, 
46, 22, 207, 
209, 47, 208, 
47, 23, 210, 
213, 212, 211, 
216, 215, 214, 
219, 218, 217, 
48, 220, 49, 
48, 222, 221, 
224, 48, 223, 
48, 226, 225, 
229, 228, 227, 
231, 48, 230, 
50, 232, 48, 
235, 234, 233, 
60, 236, 59, 
59, 238, 237, 
60, 240, 239, 
242, 60, 241, 
71, 243, 70, 
70, 245, 244, 
50, 247, 246, 
51, 248, 50, 
51, 250, 249, 
52, 251, 51, 
52, 253, 252, 
53, 254, 52, 
53, 256, 255, 
54, 257, 53, 
54, 259, 258, 
55, 260, 54, 
55, 262, 261, 
56, 263, 55, 
56, 57, 264, 
57, 58, 265, 
58, 59, 266, 
60, 268, 267, 
61, 269, 60, 
61, 271, 270, 
62, 272, 61, 
62, 274, 273, 
63, 275, 62, 
63, 277, 276, 
64, 278, 63, 
64, 280, 279, 
65, 281, 64, 
65, 283, 282, 
66, 284, 65, 
66, 286, 285, 
67, 287, 66, 
67, 68, 288, 
68, 69, 289, 
69, 70, 290, 
291, 71, 49, 
74, 73, 72, 
73, 96, 72, 
75, 74, 72, 
76, 75, 72, 
96, 95, 72, 
77, 76, 72, 
78, 77, 72, 
79, 78, 72, 
80, 79, 72, 
81, 80, 72, 
82, 81, 72, 
83, 82, 72, 
84, 83, 72, 
85, 84, 72, 
86, 85, 72, 
87, 86, 72, 
88, 87, 72, 
89, 88, 72, 
90, 89, 72, 
91, 90, 72, 
92, 91, 72, 
93, 92, 72, 
94, 93, 72, 
95, 94, 72, 
98, 292, 97, 
293, 96, 99, 
101, 294, 100, 
99, 96, 295, 
74, 102, 103, 
297, 296, 104, 
73, 105, 298, 
301, 300, 299, 
76, 106, 107, 
303, 302, 108, 
75, 109, 304, 
307, 306, 305, 
78, 110, 111, 
309, 308, 112, 
77, 113, 310, 
313, 312, 311, 
80, 114, 115, 
315, 314, 116, 
79, 117, 316, 
319, 318, 317, 
82, 118, 119, 
321, 320, 120, 
81, 121, 322, 
325, 324, 323, 
84, 122, 123, 
327, 326, 124, 
83, 125, 328, 
331, 330, 329, 
86, 126, 127, 
333, 332, 128, 
85, 129, 334, 
337, 336, 335, 
88, 130, 131, 
339, 338, 132, 
87, 133, 340, 
343, 342, 341, 
90, 134, 135, 
345, 344, 136, 
89, 137, 346, 
349, 348, 347, 
92, 138, 139, 
351, 350, 140, 
91, 141, 352, 
355, 354, 353, 
357, 356, 142, 
143, 94, 358, 
93, 144, 359, 
362, 361, 360, 
364, 363, 95, 
96, 365, 95, 
368, 367, 366, 
371, 370, 369, 
373, 372, 96, 
73, 374, 96, 
377, 376, 375, 
380, 379, 378, 
383, 382, 381, 
386, 385, 384, 
102, 74, 387, 
388, 74, 75, 
391, 390, 389, 
394, 393, 392, 
105, 73, 103, 
103, 73, 74, 
397, 396, 395, 
400, 399, 398, 
106, 76, 401, 
402, 76, 77, 
405, 404, 403, 
408, 407, 406, 
109, 75, 107, 
107, 75, 76, 
411, 410, 409, 
414, 413, 412, 
110, 78, 415, 
416, 78, 79, 
419, 418, 417, 
422, 421, 420, 
113, 77, 111, 
111, 77, 78, 
425, 424, 423, 
428, 427, 426, 
114, 80, 429, 
430, 80, 81, 
433, 432, 431, 
436, 435, 434, 
117, 79, 115, 
115, 79, 80, 
439, 438, 437, 
442, 441, 440, 
118, 82, 443, 
444, 82, 83, 
447, 446, 445, 
450, 449, 448, 
121, 81, 119, 
119, 81, 82, 
453, 452, 451, 
456, 455, 454, 
122, 84, 457, 
458, 84, 85, 
461, 460, 459, 
464, 463, 462, 
125, 83, 123, 
123, 83, 84, 
467, 466, 465, 
470, 469, 468, 
126, 86, 471, 
472, 86, 87, 
475, 474, 473, 
478, 477, 476, 
129, 85, 127, 
127, 85, 86, 
481, 480, 479, 
484, 483, 482, 
130, 88, 485, 
486, 88, 89, 
489, 488, 487, 
492, 491, 490, 
133, 87, 131, 
131, 87, 88, 
495, 494, 493, 
498, 497, 496, 
134, 90, 499, 
500, 90, 91, 
503, 502, 501, 
506, 505, 504, 
137, 89, 135, 
135, 89, 90, 
509, 508, 507, 
512, 511, 510, 
138, 92, 513, 
514, 92, 93, 
517, 516, 515, 
520, 519, 518, 
141, 91, 139, 
139, 91, 92, 
522, 521, 94, 
95, 523, 94, 
525, 142, 524, 
528, 527, 526, 
531, 530, 529, 
534, 533, 532, 
144, 93, 143, 
143, 93, 94 };
