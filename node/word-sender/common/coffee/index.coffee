# window.$ = require "jquery/dist/jquery.min"
# window.jQuery = window.$
# window._ = require "underscore/underscore-min"
# window.Backbone = require "backbone"
# window.Marionette = require "backbone.marionette/lib/backbone.marionette.min"

# require "jquery.transit/jquery.transit.js"
# require "jquery.easing/js/jquery.easing.min.js"


do (window=window, document=document, $=jQuery) ->
  "use strict"

  window.sn = $.typeApp = {}
  window.common = {}


  # ============================================================
  # TypeFrameWork
  require("./typeFrameWork/typeFrameWork.wp")(window, document, $)
  common.tf = new $.TypeFrameWork()

  # ============================================================
  # Library
  require "imports?this=>window!modernizr/modernizr"
  require "devicejs/lib/device.min.js"

  # require "jquery.transit/jquery.transit.js"
  # require "jquery-color/jquery.color.js"
  # require "jquery.easing/js/jquery.easing.min.js"

  # require "./librarys/easestepper"
  # require("./librarys/jquery.easescroller")($, window, document)


  # ============================================================
  # Detect / Normalize event names
  sn.support = {}
  sn.support.transition = do ->
    return true if $.support.transition and $.support.transform and $.fn.transition?
    return false
  sn.transitionEnabled = sn.support.transition


  $(window).load ->
    window.SETTING = require("setting")() # option {m: "production"}

    console.log SETTING

    # ============================================================
    # Module
    window.common.stage  = require( "./stage" )( sn )
    window.common.header = require( "./header" )( sn )
    window.common.footer = require( "./footer" )( sn )

    # --------------------------------------------------------------
    common.tf.setup ->
      $.when(
        window.common.stage.setup()
        window.common.header.setup()
        window.common.footer.setup()
      ).then( =>
        console.log "- SETUP COMMON JAVA SCRIPT -"
      )



    # --------------------------------------------------------------
    common.tf.update ->

    # --------------------------------------------------------------
    common.tf.draw ->
   
    # --------------------------------------------------------------
    common.tf.hover ->
   
    # --------------------------------------------------------------
    common.tf.keyPressed (key) ->
   
    # --------------------------------------------------------------
    common.tf.keyReleased (key) ->
   
    # --------------------------------------------------------------
    common.tf.click ->
   
    # --------------------------------------------------------------
    common.tf.pointerDown ->

    # --------------------------------------------------------------
    common.tf.pointerEnter ->

    # --------------------------------------------------------------
    common.tf.pointerLeave ->

    # --------------------------------------------------------------
    common.tf.pointerMoved ->

    # --------------------------------------------------------------
    common.tf.pointerOut ->

    # --------------------------------------------------------------
    common.tf.pointerOver ->

    # --------------------------------------------------------------
    common.tf.pointerUp ->
    
    # --------------------------------------------------------------
    common.tf.windowScroll (top) ->
   
    # --------------------------------------------------------------
    common.tf.windowResized (width, height) ->
      window.common.stage.windowResized()
   
    # --------------------------------------------------------------
    common.tf.fullScreenChange (full) ->