const char* editor_glade=
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
"<!DOCTYPE glade-interface SYSTEM \"glade-2.0.dtd\">"
"<!--*- mode: xml -*-->"
"<glade-interface>"
"  <widget class=\"GtkWindow\" id=\"window1\">"
"    <property name=\"events\">GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK</property>"
"    <property name=\"default_width\">800</property>"
"    <property name=\"default_height\">600</property>"
"    <child>"
"      <widget class=\"GtkVBox\" id=\"vbox1\">"
"        <property name=\"visible\">True</property>"
"        <child>"
"          <widget class=\"GtkVPaned\" id=\"vpaned1\">"
"            <property name=\"visible\">True</property>"
"            <property name=\"can_focus\">True</property>"
"            <property name=\"position\">292</property>"
"            <child>"
"              <widget class=\"GtkHBox\" id=\"hbox1\">"
"                <property name=\"visible\">True</property>"
"                <child>"
"                  <widget class=\"GtkFrame\" id=\"frame3\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"label_xalign\">0.5</property>"
"                    <property name=\"shadow_type\">GTK_SHADOW_IN</property>"
"                    <child>"
"                      <widget class=\"GtkAlignment\" id=\"alignment2\">"
"                        <property name=\"visible\">True</property>"
"                        <child>"
"                          <widget class=\"GtkImage\" id=\"image1\">"
"                            <property name=\"visible\">True</property>"
"                            <property name=\"can_focus\">True</property>"
"                            <property name=\"stock\">gtk-missing-image</property>"
"                          </widget>"
"                        </child>"
"                      </widget>"
"                    </child>"
"                    <child>"
"                      <widget class=\"GtkLabel\" id=\"label1\">"
"                        <property name=\"visible\">True</property>"
"                        <property name=\"label\" translatable=\"yes\">&lt;small&gt;&lt;b&gt;Viewport&lt;/b&gt;&lt;/small&gt;</property>"
"                        <property name=\"use_markup\">True</property>"
"                      </widget>"
"                      <packing>"
"                        <property name=\"type\">label_item</property>"
"                      </packing>"
"                    </child>"
"                  </widget>"
"                </child>"
"                <child>"
"                  <widget class=\"GtkFrame\" id=\"frame1\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"label_xalign\">0.5</property>"
"                    <property name=\"shadow_type\">GTK_SHADOW_IN</property>"
"                    <child>"
"                      <widget class=\"GtkAlignment\" id=\"alignment1\">"
"                        <property name=\"visible\">True</property>"
"                        <property name=\"xalign\">0</property>"
"                        <property name=\"yalign\">0</property>"
"                        <child>"
"                          <widget class=\"GtkVBox\" id=\"vbox2\">"
"                            <property name=\"visible\">True</property>"
"                            <child>"
"                              <widget class=\"GtkTable\" id=\"table1\">"
"                                <property name=\"width_request\">256</property>"
"                                <property name=\"visible\">True</property>"
"                                <property name=\"n_rows\">3</property>"
"                                <property name=\"n_columns\">2</property>"
"                                <child>"
"                                  <placeholder/>"
"                                </child>"
"                                <child>"
"                                  <placeholder/>"
"                                </child>"
"                                <child>"
"                                  <placeholder/>"
"                                </child>"
"                                <child>"
"                                  <placeholder/>"
"                                </child>"
"                                <child>"
"                                  <placeholder/>"
"                                </child>"
"                                <child>"
"                                  <placeholder/>"
"                                </child>"
"                              </widget>"
"                              <packing>"
"                                <property name=\"expand\">False</property>"
"                              </packing>"
"                            </child>"
"                            <child>"
"                              <widget class=\"GtkLabel\" id=\"label3\">"
"                                <property name=\"visible\">True</property>"
"                              </widget>"
"                              <packing>"
"                                <property name=\"position\">1</property>"
"                              </packing>"
"                            </child>"
"                          </widget>"
"                        </child>"
"                      </widget>"
"                    </child>"
"                    <child>"
"                      <widget class=\"GtkLabel\" id=\"label2\">"
"                        <property name=\"visible\">True</property>"
"                        <property name=\"label\" translatable=\"yes\">&lt;small&gt;&lt;b&gt;Properties&lt;/b&gt;&lt;/small&gt;</property>"
"                        <property name=\"use_markup\">True</property>"
"                      </widget>"
"                      <packing>"
"                        <property name=\"type\">label_item</property>"
"                      </packing>"
"                    </child>"
"                  </widget>"
"                  <packing>"
"                    <property name=\"expand\">False</property>"
"                    <property name=\"position\">1</property>"
"                  </packing>"
"                </child>"
"              </widget>"
"              <packing>"
"                <property name=\"resize\">False</property>"
"                <property name=\"shrink\">True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <widget class=\"GtkHPaned\" id=\"hpaned1\">"
"                <property name=\"visible\">True</property>"
"                <property name=\"can_focus\">True</property>"
"                <property name=\"position\">256</property>"
"                <child>"
"                  <widget class=\"GtkFrame\" id=\"frame4\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"label_xalign\">0</property>"
"                    <property name=\"shadow_type\">GTK_SHADOW_IN</property>"
"                    <child>"
"                      <widget class=\"GtkAlignment\" id=\"alignment3\">"
"                        <property name=\"visible\">True</property>"
"                        <child>"
"                          <widget class=\"GtkTreeView\" id=\"treeview1\">"
"                            <property name=\"visible\">True</property>"
"                            <property name=\"can_focus\">True</property>"
"                            <property name=\"events\">GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK</property>"
"                          </widget>"
"                        </child>"
"                      </widget>"
"                    </child>"
"                    <child>"
"                      <widget class=\"GtkLabel\" id=\"label6\">"
"                        <property name=\"visible\">True</property>"
"                        <property name=\"label\" translatable=\"yes\">&lt;small&gt;&lt;b&gt;Project&lt;/b&gt;&lt;/small&gt;</property>"
"                        <property name=\"use_markup\">True</property>"
"                      </widget>"
"                      <packing>"
"                        <property name=\"type\">label_item</property>"
"                      </packing>"
"                    </child>"
"                  </widget>"
"                  <packing>"
"                    <property name=\"resize\">False</property>"
"                    <property name=\"shrink\">True</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <widget class=\"GtkFrame\" id=\"frame2\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"label_xalign\">0.5</property>"
"                    <property name=\"shadow_type\">GTK_SHADOW_IN</property>"
"                    <child>"
"                      <widget class=\"GtkScrolledWindow\" id=\"scrolledwindow1\">"
"                        <property name=\"visible\">True</property>"
"                        <property name=\"can_focus\">True</property>"
"                        <property name=\"hscrollbar_policy\">GTK_POLICY_AUTOMATIC</property>"
"                        <property name=\"vscrollbar_policy\">GTK_POLICY_AUTOMATIC</property>"
"                        <child>"
"                          <widget class=\"GtkViewport\" id=\"viewport1\">"
"                            <property name=\"visible\">True</property>"
"                            <property name=\"events\">GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK</property>"
"                            <property name=\"shadow_type\">GTK_SHADOW_NONE</property>"
"                            <child>"
"                              <widget class=\"GtkFixed\" id=\"fixed1\">"
"                                <property name=\"visible\">True</property>"
"                                <property name=\"can_focus\">True</property>"
"                                <property name=\"events\">GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK</property>"
"                              </widget>"
"                            </child>"
"                          </widget>"
"                        </child>"
"                      </widget>"
"                    </child>"
"                    <child>"
"                      <widget class=\"GtkLabel\" id=\"label7\">"
"                        <property name=\"visible\">True</property>"
"                        <property name=\"label\" translatable=\"yes\">&lt;small&gt;&lt;b&gt;Operators&lt;/b&gt;&lt;/small&gt;</property>"
"                        <property name=\"use_markup\">True</property>"
"                      </widget>"
"                      <packing>"
"                        <property name=\"type\">label_item</property>"
"                      </packing>"
"                    </child>"
"                  </widget>"
"                  <packing>"
"                    <property name=\"resize\">True</property>"
"                    <property name=\"shrink\">True</property>"
"                  </packing>"
"                </child>"
"              </widget>"
"              <packing>"
"                <property name=\"resize\">True</property>"
"                <property name=\"shrink\">True</property>"
"              </packing>"
"            </child>"
"          </widget>"
"        </child>"
"        <child>"
"          <widget class=\"GtkStatusbar\" id=\"statusbar1\">"
"            <property name=\"visible\">True</property>"
"          </widget>"
"          <packing>"
"            <property name=\"expand\">False</property>"
"            <property name=\"position\">1</property>"
"          </packing>"
"        </child>"
"      </widget>"
"    </child>"
"  </widget>"
"</glade-interface>"
;
