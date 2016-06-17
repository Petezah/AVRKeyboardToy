<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.6.0">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="frames">
<description>&lt;b&gt;Frames for Sheet and Layout&lt;/b&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="FRAME_A_L">
<frame x1="0" y1="0" x2="279.4" y2="215.9" columns="6" rows="5" layer="94" border-bottom="no"/>
</symbol>
<symbol name="DOCFIELD">
<wire x1="0" y1="0" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="87.63" y2="15.24" width="0.1016" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="71.12" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="0" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="87.63" y1="5.08" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="101.6" y1="5.08" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="0" y1="15.24" x2="0" y2="22.86" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="22.86" x2="101.6" y2="15.24" width="0.1016" layer="94"/>
<text x="1.27" y="1.27" size="2.54" layer="94">Date:</text>
<text x="12.7" y="1.27" size="2.54" layer="94">&gt;LAST_DATE_TIME</text>
<text x="72.39" y="1.27" size="2.54" layer="94">Sheet:</text>
<text x="86.36" y="1.27" size="2.54" layer="94">&gt;SHEET</text>
<text x="88.9" y="11.43" size="2.54" layer="94">REV:</text>
<text x="1.27" y="19.05" size="2.54" layer="94">TITLE:</text>
<text x="1.27" y="11.43" size="2.54" layer="94">Document Number:</text>
<text x="17.78" y="19.05" size="2.54" layer="94">&gt;DRAWING_NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="FRAME_A_L" prefix="FRAME" uservalue="yes">
<description>&lt;b&gt;FRAME&lt;/b&gt; A Size , 8 1/2 x 11 INCH, Landscape&lt;p&gt;</description>
<gates>
<gate name="G$1" symbol="FRAME_A_L" x="0" y="0" addlevel="always"/>
<gate name="G$2" symbol="DOCFIELD" x="172.72" y="0" addlevel="always"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Atmel_By_element14_Batch_1-00">
<description>Developed by element14 :&lt;br&gt;
element14 CAD Library consolidation.ulp
at 27/07/2012 14:02:49</description>
<packages>
<package name="DIP254P762X457-28">
<pad name="1" x="-7.62" y="33.02" drill="1.1176" shape="square"/>
<pad name="2" x="-7.62" y="30.48" drill="1.1176"/>
<pad name="3" x="-7.62" y="27.94" drill="1.1176"/>
<pad name="4" x="-7.62" y="25.4" drill="1.1176"/>
<pad name="5" x="-7.62" y="22.86" drill="1.1176"/>
<pad name="6" x="-7.62" y="20.32" drill="1.1176"/>
<pad name="7" x="-7.62" y="17.78" drill="1.1176"/>
<pad name="8" x="-7.62" y="15.24" drill="1.1176"/>
<pad name="9" x="-7.62" y="12.7" drill="1.1176"/>
<pad name="10" x="-7.62" y="10.16" drill="1.1176"/>
<pad name="11" x="-7.62" y="7.62" drill="1.1176"/>
<pad name="12" x="-7.62" y="5.08" drill="1.1176"/>
<pad name="13" x="-7.62" y="2.54" drill="1.1176"/>
<pad name="14" x="-7.62" y="0" drill="1.1176"/>
<pad name="15" x="0" y="0" drill="1.1176"/>
<pad name="16" x="0" y="2.54" drill="1.1176"/>
<pad name="17" x="0" y="5.08" drill="1.1176"/>
<pad name="18" x="0" y="7.62" drill="1.1176"/>
<pad name="19" x="0" y="10.16" drill="1.1176"/>
<pad name="20" x="0" y="12.7" drill="1.1176"/>
<pad name="21" x="0" y="15.24" drill="1.1176"/>
<pad name="22" x="0" y="17.78" drill="1.1176"/>
<pad name="23" x="0" y="20.32" drill="1.1176"/>
<pad name="24" x="0" y="22.86" drill="1.1176"/>
<pad name="25" x="0" y="25.4" drill="1.1176"/>
<pad name="26" x="0" y="27.94" drill="1.1176"/>
<pad name="27" x="0" y="30.48" drill="1.1176"/>
<pad name="28" x="0" y="33.02" drill="1.1176"/>
<wire x1="-6.6548" y1="-0.889" x2="-0.9652" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="-0.9652" y1="33.909" x2="-3.5052" y2="33.909" width="0.1524" layer="21"/>
<wire x1="-3.5052" y1="33.909" x2="-4.1148" y2="33.909" width="0.1524" layer="21"/>
<wire x1="-4.1148" y1="33.909" x2="-6.2992" y2="33.909" width="0.1524" layer="21"/>
<wire x1="-3.5052" y1="33.909" x2="-4.1148" y2="33.909" width="0.1524" layer="21" curve="-180"/>
<text x="-8.2042" y="34.1376" size="1.27" layer="21" ratio="6" rot="SR0">*</text>
<wire x1="-7.5692" y1="32.4612" x2="-7.5692" y2="33.5788" width="0" layer="51"/>
<wire x1="-7.5692" y1="33.5788" x2="-8.1788" y2="33.5788" width="0" layer="51"/>
<wire x1="-8.1788" y1="33.5788" x2="-8.1788" y2="32.4612" width="0" layer="51"/>
<wire x1="-8.1788" y1="32.4612" x2="-7.5692" y2="32.4612" width="0" layer="51"/>
<wire x1="-7.5692" y1="29.9212" x2="-7.5692" y2="31.0388" width="0" layer="51"/>
<wire x1="-7.5692" y1="31.0388" x2="-8.1788" y2="31.0388" width="0" layer="51"/>
<wire x1="-8.1788" y1="31.0388" x2="-8.1788" y2="29.9212" width="0" layer="51"/>
<wire x1="-8.1788" y1="29.9212" x2="-7.5692" y2="29.9212" width="0" layer="51"/>
<wire x1="-7.5692" y1="27.3812" x2="-7.5692" y2="28.4988" width="0" layer="51"/>
<wire x1="-7.5692" y1="28.4988" x2="-8.1788" y2="28.4988" width="0" layer="51"/>
<wire x1="-8.1788" y1="28.4988" x2="-8.1788" y2="27.3812" width="0" layer="51"/>
<wire x1="-8.1788" y1="27.3812" x2="-7.5692" y2="27.3812" width="0" layer="51"/>
<wire x1="-7.5692" y1="24.8412" x2="-7.5692" y2="25.9588" width="0" layer="51"/>
<wire x1="-7.5692" y1="25.9588" x2="-8.1788" y2="25.9588" width="0" layer="51"/>
<wire x1="-8.1788" y1="25.9588" x2="-8.1788" y2="24.8412" width="0" layer="51"/>
<wire x1="-8.1788" y1="24.8412" x2="-7.5692" y2="24.8412" width="0" layer="51"/>
<wire x1="-7.5692" y1="22.3012" x2="-7.5692" y2="23.4188" width="0" layer="51"/>
<wire x1="-7.5692" y1="23.4188" x2="-8.1788" y2="23.4188" width="0" layer="51"/>
<wire x1="-8.1788" y1="23.4188" x2="-8.1788" y2="22.3012" width="0" layer="51"/>
<wire x1="-8.1788" y1="22.3012" x2="-7.5692" y2="22.3012" width="0" layer="51"/>
<wire x1="-7.5692" y1="19.7612" x2="-7.5692" y2="20.8788" width="0" layer="51"/>
<wire x1="-7.5692" y1="20.8788" x2="-8.1788" y2="20.8788" width="0" layer="51"/>
<wire x1="-8.1788" y1="20.8788" x2="-8.1788" y2="19.7612" width="0" layer="51"/>
<wire x1="-8.1788" y1="19.7612" x2="-7.5692" y2="19.7612" width="0" layer="51"/>
<wire x1="-7.5692" y1="17.2212" x2="-7.5692" y2="18.3388" width="0" layer="51"/>
<wire x1="-7.5692" y1="18.3388" x2="-8.1788" y2="18.3388" width="0" layer="51"/>
<wire x1="-8.1788" y1="18.3388" x2="-8.1788" y2="17.2212" width="0" layer="51"/>
<wire x1="-8.1788" y1="17.2212" x2="-7.5692" y2="17.2212" width="0" layer="51"/>
<wire x1="-7.5692" y1="14.6812" x2="-7.5692" y2="15.7988" width="0" layer="51"/>
<wire x1="-7.5692" y1="15.7988" x2="-8.1788" y2="15.7988" width="0" layer="51"/>
<wire x1="-8.1788" y1="15.7988" x2="-8.1788" y2="14.6812" width="0" layer="51"/>
<wire x1="-8.1788" y1="14.6812" x2="-7.5692" y2="14.6812" width="0" layer="51"/>
<wire x1="-7.5438" y1="12.1412" x2="-7.5692" y2="13.2588" width="0" layer="51"/>
<wire x1="-7.5692" y1="13.2588" x2="-8.1788" y2="13.2588" width="0" layer="51"/>
<wire x1="-8.1788" y1="13.2588" x2="-8.1788" y2="12.1412" width="0" layer="51"/>
<wire x1="-8.1788" y1="12.1412" x2="-7.5438" y2="12.1412" width="0" layer="51"/>
<wire x1="-7.5438" y1="9.6012" x2="-7.5438" y2="10.7188" width="0" layer="51"/>
<wire x1="-7.5438" y1="10.7188" x2="-8.1788" y2="10.7188" width="0" layer="51"/>
<wire x1="-8.1788" y1="10.7188" x2="-8.1788" y2="9.6012" width="0" layer="51"/>
<wire x1="-8.1788" y1="9.6012" x2="-7.5438" y2="9.6012" width="0" layer="51"/>
<wire x1="-7.5438" y1="7.0612" x2="-7.5438" y2="8.1788" width="0" layer="51"/>
<wire x1="-7.5438" y1="8.1788" x2="-8.1788" y2="8.1788" width="0" layer="51"/>
<wire x1="-8.1788" y1="8.1788" x2="-8.1788" y2="7.0612" width="0" layer="51"/>
<wire x1="-8.1788" y1="7.0612" x2="-7.5438" y2="7.0612" width="0" layer="51"/>
<wire x1="-7.5438" y1="4.5212" x2="-7.5438" y2="5.6388" width="0" layer="51"/>
<wire x1="-7.5438" y1="5.6388" x2="-8.1788" y2="5.6388" width="0" layer="51"/>
<wire x1="-8.1788" y1="5.6388" x2="-8.1788" y2="4.5212" width="0" layer="51"/>
<wire x1="-8.1788" y1="4.5212" x2="-7.5438" y2="4.5212" width="0" layer="51"/>
<wire x1="-7.5438" y1="1.9812" x2="-7.5438" y2="3.0988" width="0" layer="51"/>
<wire x1="-7.5438" y1="3.0988" x2="-8.1788" y2="3.0988" width="0" layer="51"/>
<wire x1="-8.1788" y1="3.0988" x2="-8.1788" y2="1.9812" width="0" layer="51"/>
<wire x1="-8.1788" y1="1.9812" x2="-7.5438" y2="1.9812" width="0" layer="51"/>
<wire x1="-7.5438" y1="-0.5588" x2="-7.5438" y2="0.5588" width="0" layer="51"/>
<wire x1="-7.5438" y1="0.5588" x2="-8.1788" y2="0.5588" width="0" layer="51"/>
<wire x1="-8.1788" y1="0.5588" x2="-8.1788" y2="-0.5588" width="0" layer="51"/>
<wire x1="-8.1788" y1="-0.5588" x2="-7.5438" y2="-0.5588" width="0" layer="51"/>
<wire x1="-0.0762" y1="0.5588" x2="-0.0508" y2="-0.5588" width="0" layer="51"/>
<wire x1="-0.0508" y1="-0.5588" x2="0.5588" y2="-0.5588" width="0" layer="51"/>
<wire x1="0.5588" y1="-0.5588" x2="0.5588" y2="0.5588" width="0" layer="51"/>
<wire x1="0.5588" y1="0.5588" x2="-0.0762" y2="0.5588" width="0" layer="51"/>
<wire x1="-0.0762" y1="3.0988" x2="-0.0508" y2="1.9812" width="0" layer="51"/>
<wire x1="-0.0508" y1="1.9812" x2="0.5588" y2="1.9812" width="0" layer="51"/>
<wire x1="0.5588" y1="1.9812" x2="0.5588" y2="3.0988" width="0" layer="51"/>
<wire x1="0.5588" y1="3.0988" x2="-0.0762" y2="3.0988" width="0" layer="51"/>
<wire x1="-0.0762" y1="5.6388" x2="-0.0762" y2="4.5212" width="0" layer="51"/>
<wire x1="-0.0762" y1="4.5212" x2="0.5588" y2="4.5212" width="0" layer="51"/>
<wire x1="0.5588" y1="4.5212" x2="0.5588" y2="5.6388" width="0" layer="51"/>
<wire x1="0.5588" y1="5.6388" x2="-0.0762" y2="5.6388" width="0" layer="51"/>
<wire x1="-0.0762" y1="8.1788" x2="-0.0762" y2="7.0612" width="0" layer="51"/>
<wire x1="-0.0762" y1="7.0612" x2="0.5588" y2="7.0612" width="0" layer="51"/>
<wire x1="0.5588" y1="7.0612" x2="0.5588" y2="8.1788" width="0" layer="51"/>
<wire x1="0.5588" y1="8.1788" x2="-0.0762" y2="8.1788" width="0" layer="51"/>
<wire x1="-0.0762" y1="10.7188" x2="-0.0762" y2="9.6012" width="0" layer="51"/>
<wire x1="-0.0762" y1="9.6012" x2="0.5588" y2="9.6012" width="0" layer="51"/>
<wire x1="0.5588" y1="9.6012" x2="0.5588" y2="10.7188" width="0" layer="51"/>
<wire x1="0.5588" y1="10.7188" x2="-0.0762" y2="10.7188" width="0" layer="51"/>
<wire x1="-0.0762" y1="13.2588" x2="-0.0762" y2="12.1412" width="0" layer="51"/>
<wire x1="-0.0762" y1="12.1412" x2="0.5588" y2="12.1412" width="0" layer="51"/>
<wire x1="0.5588" y1="12.1412" x2="0.5588" y2="13.2588" width="0" layer="51"/>
<wire x1="0.5588" y1="13.2588" x2="-0.0762" y2="13.2588" width="0" layer="51"/>
<wire x1="-0.0762" y1="15.7988" x2="-0.0762" y2="14.6812" width="0" layer="51"/>
<wire x1="-0.0762" y1="14.6812" x2="0.5588" y2="14.6812" width="0" layer="51"/>
<wire x1="0.5588" y1="14.6812" x2="0.5588" y2="15.7988" width="0" layer="51"/>
<wire x1="0.5588" y1="15.7988" x2="-0.0762" y2="15.7988" width="0" layer="51"/>
<wire x1="-0.0762" y1="18.3388" x2="-0.0762" y2="17.2212" width="0" layer="51"/>
<wire x1="-0.0762" y1="17.2212" x2="0.5588" y2="17.2212" width="0" layer="51"/>
<wire x1="0.5588" y1="17.2212" x2="0.5588" y2="18.3388" width="0" layer="51"/>
<wire x1="0.5588" y1="18.3388" x2="-0.0762" y2="18.3388" width="0" layer="51"/>
<wire x1="-0.0762" y1="20.8788" x2="-0.0762" y2="19.7612" width="0" layer="51"/>
<wire x1="-0.0762" y1="19.7612" x2="0.5588" y2="19.7612" width="0" layer="51"/>
<wire x1="0.5588" y1="19.7612" x2="0.5588" y2="20.8788" width="0" layer="51"/>
<wire x1="0.5588" y1="20.8788" x2="-0.0762" y2="20.8788" width="0" layer="51"/>
<wire x1="-0.0762" y1="23.4188" x2="-0.0762" y2="22.3012" width="0" layer="51"/>
<wire x1="-0.0762" y1="22.3012" x2="0.5588" y2="22.3012" width="0" layer="51"/>
<wire x1="0.5588" y1="22.3012" x2="0.5588" y2="23.4188" width="0" layer="51"/>
<wire x1="0.5588" y1="23.4188" x2="-0.0762" y2="23.4188" width="0" layer="51"/>
<wire x1="-0.0762" y1="25.9588" x2="-0.0762" y2="24.8412" width="0" layer="51"/>
<wire x1="-0.0762" y1="24.8412" x2="0.5588" y2="24.8412" width="0" layer="51"/>
<wire x1="0.5588" y1="24.8412" x2="0.5588" y2="25.9588" width="0" layer="51"/>
<wire x1="0.5588" y1="25.9588" x2="-0.0762" y2="25.9588" width="0" layer="51"/>
<wire x1="-0.0762" y1="28.4988" x2="-0.0762" y2="27.3812" width="0" layer="51"/>
<wire x1="-0.0762" y1="27.3812" x2="0.5588" y2="27.3812" width="0" layer="51"/>
<wire x1="0.5588" y1="27.3812" x2="0.5588" y2="28.4988" width="0" layer="51"/>
<wire x1="0.5588" y1="28.4988" x2="-0.0762" y2="28.4988" width="0" layer="51"/>
<wire x1="-0.0762" y1="31.0388" x2="-0.0762" y2="29.9212" width="0" layer="51"/>
<wire x1="-0.0762" y1="29.9212" x2="0.5588" y2="29.9212" width="0" layer="51"/>
<wire x1="0.5588" y1="29.9212" x2="0.5588" y2="31.0388" width="0" layer="51"/>
<wire x1="0.5588" y1="31.0388" x2="-0.0762" y2="31.0388" width="0" layer="51"/>
<wire x1="-0.0762" y1="33.5788" x2="-0.0762" y2="32.4612" width="0" layer="51"/>
<wire x1="-0.0762" y1="32.4612" x2="0.5588" y2="32.4612" width="0" layer="51"/>
<wire x1="0.5588" y1="32.4612" x2="0.5588" y2="33.5788" width="0" layer="51"/>
<wire x1="0.5588" y1="33.5788" x2="-0.0762" y2="33.5788" width="0" layer="51"/>
<wire x1="-7.5692" y1="-0.889" x2="-0.0508" y2="-0.889" width="0" layer="51"/>
<wire x1="-0.0508" y1="-0.889" x2="-0.0508" y2="33.909" width="0" layer="51"/>
<wire x1="-0.0508" y1="33.909" x2="-3.5052" y2="33.909" width="0" layer="51"/>
<wire x1="-3.5052" y1="33.909" x2="-4.1148" y2="33.909" width="0" layer="51"/>
<wire x1="-4.1148" y1="33.909" x2="-7.5692" y2="33.909" width="0" layer="51"/>
<wire x1="-7.5692" y1="33.909" x2="-7.5692" y2="-0.889" width="0" layer="51"/>
<wire x1="-3.5052" y1="33.909" x2="-4.1148" y2="33.909" width="0" layer="51" curve="-180"/>
<text x="-8.2042" y="34.1376" size="1.27" layer="51" ratio="6" rot="SR0">*</text>
<text x="-8.7376" y="-5.8166" size="2.0828" layer="25" ratio="10" rot="SR0">&gt;NAME</text>
<text x="-7.2644" y="36.195" size="2.0828" layer="27" ratio="10" rot="SR0">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="ATMEGA328P-PU">
<pin name="VCC" x="-17.78" y="15.24" length="middle" direction="pwr"/>
<pin name="AVCC" x="-17.78" y="12.7" length="middle" direction="pwr"/>
<pin name="AREF" x="-17.78" y="7.62" length="middle" direction="in"/>
<pin name="PB0" x="-17.78" y="2.54" length="middle"/>
<pin name="PB1" x="-17.78" y="0" length="middle"/>
<pin name="PB2" x="-17.78" y="-2.54" length="middle"/>
<pin name="PB3" x="-17.78" y="-5.08" length="middle"/>
<pin name="PB4" x="-17.78" y="-7.62" length="middle"/>
<pin name="PB5" x="-17.78" y="-10.16" length="middle"/>
<pin name="PB6" x="-17.78" y="-12.7" length="middle"/>
<pin name="PB7" x="-17.78" y="-15.24" length="middle"/>
<pin name="GND_2" x="-17.78" y="-20.32" length="middle" direction="pas"/>
<pin name="GND" x="-17.78" y="-22.86" length="middle" direction="pas"/>
<pin name="PD0" x="17.78" y="15.24" length="middle" rot="R180"/>
<pin name="PD1" x="17.78" y="12.7" length="middle" rot="R180"/>
<pin name="PD2" x="17.78" y="10.16" length="middle" rot="R180"/>
<pin name="PD3" x="17.78" y="7.62" length="middle" rot="R180"/>
<pin name="PD4" x="17.78" y="5.08" length="middle" rot="R180"/>
<pin name="PD5" x="17.78" y="2.54" length="middle" rot="R180"/>
<pin name="PD6" x="17.78" y="0" length="middle" rot="R180"/>
<pin name="PD7" x="17.78" y="-2.54" length="middle" rot="R180"/>
<pin name="PC0" x="17.78" y="-7.62" length="middle" rot="R180"/>
<pin name="PC1" x="17.78" y="-10.16" length="middle" rot="R180"/>
<pin name="PC2" x="17.78" y="-12.7" length="middle" rot="R180"/>
<pin name="PC3" x="17.78" y="-15.24" length="middle" rot="R180"/>
<pin name="PC4" x="17.78" y="-17.78" length="middle" rot="R180"/>
<pin name="PC5" x="17.78" y="-20.32" length="middle" rot="R180"/>
<pin name="PC6" x="17.78" y="-22.86" length="middle" rot="R180"/>
<wire x1="-12.7" y1="20.32" x2="-12.7" y2="-27.94" width="0.4064" layer="94"/>
<wire x1="-12.7" y1="-27.94" x2="12.7" y2="-27.94" width="0.4064" layer="94"/>
<wire x1="12.7" y1="-27.94" x2="12.7" y2="20.32" width="0.4064" layer="94"/>
<wire x1="12.7" y1="20.32" x2="-12.7" y2="20.32" width="0.4064" layer="94"/>
<text x="-5.5626" y="21.5138" size="2.0828" layer="95" ratio="10" rot="SR0">&gt;NAME</text>
<text x="-4.9784" y="-31.0388" size="2.0828" layer="96" ratio="10" rot="SR0">&gt;VALUE</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="ATMEGA328P-PU" prefix="U">
<description>8-bit Microcontroller with In-System Programmable Flash</description>
<gates>
<gate name="A" symbol="ATMEGA328P-PU" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DIP254P762X457-28">
<connects>
<connect gate="A" pin="AREF" pad="21"/>
<connect gate="A" pin="AVCC" pad="20"/>
<connect gate="A" pin="GND" pad="22"/>
<connect gate="A" pin="GND_2" pad="8"/>
<connect gate="A" pin="PB0" pad="14"/>
<connect gate="A" pin="PB1" pad="15"/>
<connect gate="A" pin="PB2" pad="16"/>
<connect gate="A" pin="PB3" pad="17"/>
<connect gate="A" pin="PB4" pad="18"/>
<connect gate="A" pin="PB5" pad="19"/>
<connect gate="A" pin="PB6" pad="9"/>
<connect gate="A" pin="PB7" pad="10"/>
<connect gate="A" pin="PC0" pad="23"/>
<connect gate="A" pin="PC1" pad="24"/>
<connect gate="A" pin="PC2" pad="25"/>
<connect gate="A" pin="PC3" pad="26"/>
<connect gate="A" pin="PC4" pad="27"/>
<connect gate="A" pin="PC5" pad="28"/>
<connect gate="A" pin="PC6" pad="1"/>
<connect gate="A" pin="PD0" pad="2"/>
<connect gate="A" pin="PD1" pad="3"/>
<connect gate="A" pin="PD2" pad="4"/>
<connect gate="A" pin="PD3" pad="5"/>
<connect gate="A" pin="PD4" pad="6"/>
<connect gate="A" pin="PD5" pad="11"/>
<connect gate="A" pin="PD6" pad="12"/>
<connect gate="A" pin="PD7" pad="13"/>
<connect gate="A" pin="VCC" pad="7"/>
</connects>
<technologies>
<technology name="">
<attribute name="MPN" value="ATMEGA328P-PU" constant="no"/>
<attribute name="OC_FARNELL" value="1715487" constant="no"/>
<attribute name="OC_NEWARK" value="15R0268" constant="no"/>
<attribute name="PACKAGE" value="PDIP-28" constant="no"/>
<attribute name="SUPPLIER" value="Atmel" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="FRAME1" library="frames" deviceset="FRAME_A_L" device=""/>
<part name="U1" library="Atmel_By_element14_Batch_1-00" deviceset="ATMEGA328P-PU" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="FRAME1" gate="G$1" x="-17.78" y="-10.16"/>
<instance part="FRAME1" gate="G$2" x="154.94" y="-10.16"/>
<instance part="U1" gate="A" x="58.42" y="101.6"/>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
