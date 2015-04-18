# console.log $
# console.log _
# console.log Backbone

# $(window).load (window, document, $=jQuery) ->
# do (window, document, $=jQuery) ->

do (window=window, document=document, $=jQuery) ->
  "use strict"

  # ============================================================
  # TypeFrameWork
  sn.tf = new TypeFrameWork()

  # ============================================================
  # Library
  # require "greensock/src/minified/jquery.gsap.min.js"
  # require "greensock/src/minified/TweenMax.min.js"

  require "jquery.transit/jquery.transit.js"
  require "jquery.easing/js/jquery.easing.min.js"
  require "jquery.quantize"

  # require("./typeCanvas/typeCanvas.wp")

  # console.log TweenMax

  # ============================================================
  # Detect / Normalize event names
  sn.support = {}
  sn.support.transition = do ->
    return true if $.support.transition and $.support.transform and $.fn.transition?
    return false
  sn.transitionEnabled = sn.support.transition

  # ============================================================
  # Module
  # require("./router")(sn, $, Backbone, _)

  $(window).load ->
    console.log SETTING
    
    # window.SETTING = require( "../../config/setting" )()

    # ============================================================
    # Library
    require "jquery.transit/jquery.transit.js"
    require "jquery.easing/js/jquery.easing.min.js"
    require "jquery.quantize"

    # ============================================================
    # Module
    sn.stage = require( "./stage" )( sn )
    # sn.clock = require( "./clock" )( sn )

    # ============================================================
    # PAGE

    # ============================================================
    # POPUP


    # ============================================================
    # ROUTER
    # sn.router = new sn.Router()
 
    # --------------------------------------------------------------
    sn.tf.setup ->
      # add the renderer view element to the DOM
      # document.body.appendChild(renderer.view);


      # font-size の可変を許可するか
      common.stage.setWindowResizedEnabled( false )
      # common.setWindowResizedEnabled( true )
      common.stage.windowResized()

      $.when(
        sn.stage.setup()
        # sn.clock.setup()
      ).then( =>
        $("body").removeClass "is-invisibility"
        console.log  "- SETUP SCRIPT -"
      )


    # --------------------------------------------------------------
    sn.tf.update ->

    # --------------------------------------------------------------
    sn.tf.draw ->
   
    # --------------------------------------------------------------
    sn.tf.hover ->
   
    # --------------------------------------------------------------
    sn.tf.keyPressed (key) ->
   
    # --------------------------------------------------------------
    sn.tf.keyReleased (key) ->
   
    # --------------------------------------------------------------
    sn.tf.click ->
   
    # --------------------------------------------------------------
    sn.tf.pointerDown ->

    # --------------------------------------------------------------
    sn.tf.pointerEnter ->

    # --------------------------------------------------------------
    sn.tf.pointerLeave ->

    # --------------------------------------------------------------
    sn.tf.pointerMoved ->

    # --------------------------------------------------------------
    sn.tf.pointerOut ->

    # --------------------------------------------------------------
    sn.tf.pointerOver ->

    # --------------------------------------------------------------
    sn.tf.pointerUp ->
    
    # --------------------------------------------------------------
    sn.tf.windowScroll (top) ->
      sn.stage.windowScroll top

    # --------------------------------------------------------------
    sn.tf.windowResized (width, height) ->
      common.stage.windowResized()
      sn.stage.windowResized width, height
   
    # --------------------------------------------------------------
    sn.tf.fullScreenChange (full) ->