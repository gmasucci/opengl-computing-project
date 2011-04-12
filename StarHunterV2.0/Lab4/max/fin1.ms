--ChangeLog
---------------
--V1.1 - 05/04/11 - Allowed specifying whether to move/rotate objects - simple check boxes
--V1.2 - 08/04/11 - Re-write to allow exporting vertices per face to allow multiple normals at the same vertex position.

fn ne rot mov =
(
	if $ == undefined then(
		messagebox "Must select an object"
	)else(
		
	for obj in $ do(
	
		if classof obj != Editable_Mesh then(
		messagebox "Object must be an editable mesh"
		)else(
		
			if mov==true do(
			move obj [0,10,0]
			)
			if rot==true do(
			ea = eulerAngles -90 0 0
			rotate obj ea
			)
		
	out_name = getsavefilename caption:"Export UWSM" types:"UWSM mesh(*.UWSM)|*.uwsm"
	file = createfile out_name
	typever = "uwsm1.2;"
	format "%" typever to:file
	
		--debug
		obj = $
		nv = obj.numverts
		nf = obj.numfaces
		nv3 = nv*3	--verts * 3 for xyz components
		totalv = nv3*3
			
		verts = #()
		norms = #()
		texcd = #()
		faces = #()
		txfaces = #()
		jim = (meshop.getNumMapVerts obj 1)*2
		
		for f = 1 to obj.numfaces do
		(
			--f=1
			face = getFace obj f
			v1 = getVert obj face.x
			v2 = getVert obj face.y
			v3 = getVert obj face.z
			
			append verts v1.x
			append verts v1.y
			append verts v1.z
			
			append verts v2.x
			append verts v2.y
			append verts v2.z
			
			append verts v3.x
			append verts v3.y
			append verts v3.z
			
			
			norm = getFaceNormal obj f
			for i = 1 to 3 do(
				append norms norm.x
				append norms norm.y
				append norms norm.z
			)
			
		)			
		
		jim = (meshop.getNumMapVerts obj 1)*2
		for t = 1 to meshop.getNumMapVerts obj 1 do(
				tv = meshop.getMapVert obj 1 t
				append texcd tv.x
				append texcd tv.y
		)
		
		jim = meshop.getNumMapFaces obj 1

			for t = 1 to jim do(
				tf = meshop.getMapFace obj 1 t
				
				tfx = tf.x as Integer
				tfy = tf.y as Integer
				tfz = tf.z as Integer
				
				tfx-=1
				tfy-=1
				tfz-=1
				
				append txfaces tfx
				append txfaces tfy
				append txfaces tfz			
			)
		
		
		for f = 1 to obj.numfaces*3 do(
					faces[f] = f-1
		)
		
	
		format "\n%\n" verts.count to:file
		for i = 1 to verts.count do (
			format "% " verts[i] to:file
		)
		format "\n%\n" norms.count to:file
		for i = 1 to norms.count do (
			format "% " norms[i] to:file
		)
		format "\n%\n" texcd.count to:file
		for i = 1 to texcd.count do (
			format "% " texcd[i] to:file
		)
		format "\n%\n" txfaces.count to:file
		for i = 1 to txfaces.count do (
			format "% " faces[i] to:file
			format "% " txfaces[i] to:file
		)
		format "\n\n" to: file
		
		if mov==true do(
		move obj [0,-10,0]
		)
		if rot==true do(
		ea = eulerAngles 90 0 0
		rotate obj ea
		)
		
		)
		)
	)
	messagebox "Done. New"	
	
)
fn Export rot mov =
(
	if $ == undefined then(
		messagebox "Must select an object"
	)else(
		
	
	--for each object
	for obj in $ do(
		
		if classof obj != Editable_Mesh then(
		messagebox "Object must be an editable mesh"
		)else(

		out_name = getsavefilename caption:"Export UWSM" types:"UWSM mesh(*.UWSM)|*.uwsm"
		ofile = createfile out_name
		typever = "uwsm1.2;\n" as string
		format "%" typever to:ofile
		
			if mov==true do(
			move obj [0,10,0]
			)
			if rot==true do(
			ea = eulerAngles -90 0 0
			rotate obj ea
			)
		
		nv = obj.numverts *3
		--get number or verts & bung into file
		print nv to: ofile
		--per vertex in mesh -- need to get teh verts from the face!

		for v = 1 to obj.numverts do(
			--get vertex to operate on
			vert = getVert obj v
			--output to file in specific format
			format "% % % " vert.x vert.y vert.z to: ofile
		)
		
		--normals
		format "\n%\n" nv to: ofile
		for n = 1 to obj.numverts do(
			--norm = getfacenormal obj n
			norm = getNormal obj n
			nl = length norm
			norm/=nl
			format "% % % " norm.x norm.y norm.z to: ofile
			--format "%: \"% % %\" len %\n" n norm.x norm.y norm.z nl
		)
		--texcoords
			jim = (meshop.getNumMapVerts obj 1)*2
			format "\n%\n" jim to: ofile
			for t = 1 to meshop.getNumMapVerts obj 1 do(
				tv = meshop.getMapVert obj 1 t
				format "% % " tv.x tv.y to: ofile
			)
			
			--face indices
			
			facearr = #()
			texarr = #()
			
			--faces/tris into array
			format "\n%\n" obj.numfaces to: ofile
			for f = 1 to obj.numfaces do(
				face = getFace obj f
				--extra formatting
				
				fx = face.x as Integer
				fy = face.y as Integer
				fz = face.z as Integer
				
				--take away 1 due to maxscript starting arrays at 1 not 0
				fx-=1
				fy-=1
				fz-=1
				
				append facearr fx
				append facearr fy
				append facearr fz	
		
					--format "% % % " fx fy fz to: ofile
			)
			
					
			--tex indices into array
			jim = meshop.getNumMapFaces obj 1

			for t = 1 to jim do(
				tf = meshop.getMapFace obj 1 t
				
				tfx = tf.x as Integer
				tfy = tf.y as Integer
				tfz = tf.z as Integer
				
				tfx-=1
				tfy-=1
				tfz-=1
				
				append texarr tfx
				append texarr tfy
				append texarr tfz			
			)
			
					if jim == obj.numfaces then(
				jim = obj.numfaces * 3
				for i = 1 to jim do(
					
					format "% " facearr[i] to: ofile
					format "% " texarr[i] to: ofile
			
				)
				
			)else(
				--do nothing
			)
		
		format "\n\n" to: ofile
		
		if mov==true do(
		move obj [0,-10,0]
		)
		if rot==true do(
		ea = eulerAngles 90 0 0
		rotate obj ea
		)
	)
	messagebox "Done. orig"		
	)
		
	
		--edit out_name
	)
	
	
	
)


rollout uwsmRoll "UWSM Export" width:136 height:256
(
	button btn1 "Export..." pos:[8,184] width:120 height:32 toolTip:"Export"
	--label lbltemp "Coming soon." pos:[32,253] width:88 height:24

	button info_button "Info" pos:[8,216] width:120 height:32 toolTip:"About"
	checkbox rot "Change Z to Y" pos:[16,120] width:104 height:16 enabled:true checked:true
	checkbox mov "Move +10 in Y" pos:[16,136] width:104 height:16 enabled:true checked:false
	checkbox chk3 "Seperate Faces" pos:[16,24] width:96 height:16
	label lbl1 "(Prevents normal problems on surfaces with sharp corners, Don't use on curved meshes)" pos:[16,40] width:104 height:72
	GroupBox grp1 "Options" pos:[8,8] width:120 height:160
	
	on btn1 pressed do
	(
			if chk3.checked == false then
			(Export rot.checked mov.checked)
			else
			(ne rot.checked mov.checked)
	)
	on info_button pressed do
	(
		messagebox "Finbob's UWSM Exporter  V1.2\n31/03/2011\n(C) 2011, Neil Finlay\nVisit http://finlaybob.wordpress.com"
	)
)

-- create the rollout window and add the  rollout
if uwsmFloater != undefined do
(
	closerolloutfloater uwsmFloater 
)
uwsmFloater = newRolloutFloater "UWSM Exporter" 150 285
addRollout uwsmRoll uwsmFloater 