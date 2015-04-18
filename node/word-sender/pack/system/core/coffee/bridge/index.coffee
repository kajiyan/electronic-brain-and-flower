# ============================================================
# Bridge
module.exports = (sn, $, _) ->
  # ============================================================
  # Librarys
  # require("./libraries/binaryajax")
  # require("./libraries/exif")
  # require("./libraries/canvasResize")

  # console.log canvasResize

  class Bridge extends Backbone.Model
    constructor: (options) ->
      console.log "Bridge-> constructor"

      # クラスの継承
      super

      # ============================================================
      # BackBone Models
      sn.__client__ =
        models: {}
        collections: {}
        views: {}

      # ============================================================
      # Models
      # MODEL
      

      # COLLECTION


      # VIEW


    # ------------------------------------------------------------
    initialize: () ->
      console.log "Bridge -> initialize"

      @_setting = SETTING
      @_socket = null;


    # ------------------------------------------------------------
    setup: () ->
      console.log "Bridge -> setup"

      @_connect()

      for key of sn.__client__.models
        sn.__client__.models[key].setup?()


    # ------------------------------------------------------------
    _connect: () ->
      console.log "Bridge -> _connect"

      # 接続が確立されていたら何も処理しない
      if @_socket then return

      socket = io.connect "http://#{@_setting.HOST}:#{@_setting.PORT}/bridge"

      socket.on "connect", @_connectHandler.bind(@)
      socket.on "trigger", @_triggerHandler.bind(@)


    # ------------------------------------------------------------
    _connectHandler: () ->
      console.log "Bridge -> Socket -> _connectHandler"

      @set "connected", true


    # ------------------------------------------------------------
    _triggerHandler: () ->
      console.log "Bridge -> Socket -> _connectTrigger"


  return Instance = new Bridge(sn, $, _)





# # console.log TypeEvent
# # console.log $.TypeFrameWork
# # console.log $.TypeThread

# # console.log $
# # console.log _
# # console.log Backbone

# # $(window).load (window, document, $=jQuery) ->
# # do (window, document, $=jQuery) ->

# # $(window).load ->
# # do () ->
# (data) ->
#   "use strict"

#   # sn = $.typeApp = {}
#   # sn.SETTING = require "setting"

#   # # ============================================================
#   # # TypeFrameWork
#   # require("./typeFrameWork.wp")(window, document, $)
#   # sn.tf = new $.TypeFrameWork()

#   # # ============================================================
#   # # Library

#   # # ============================================================
#   # # Models
#   # sn.context = do ->
#   #   Context = require("./models/context")
#   #   return new Context(sn, $, _)
#   # sn.images = do ->
#   #   Images = require("./collection/images")(sn, $, _)
#   #   return new Images()


#   # # ============================================================
#   # # Views
#   # sn.views =
#   #   header: do ->
#   #     Header = require("./views/header")(sn, $, _)
#   #     return new Header()
#   #   footer: do ->
#   #     Footer = require("./views/footer")(sn, $, _)
#   #     return new Footer()


#   # # ============================================================
#   # # Router
#   # sn.router = do ->
#   #   Router = require("./router/index")(sn, $, _)
#   #   return new Router()
    

#   $ ->
#     # ============================================================
#     # StageClass ( Test Class )
#     class sn.Stage extends TypeEvent
#       constructor: () ->
#         super
#         @
 
#     # ============================================================
#     # Module

#     # ============================================================
#     # PAGE

#     # ============================================================
#     # POPUP
 
#     # --------------------------------------------------------------
#     sn.tf.setup ->
#       # $.when(
#       #   sn.router.setup()
#       # ).then( =>
#       #   # sn.context.setup()
#       #   # sn.images.setup()
#       # ).then( =>
#       #   # sn.context.run()
#       #   console.log "- SETUP APP -"
#       # )

#     # --------------------------------------------------------------
#     sn.tf.update ->

#     # --------------------------------------------------------------
#     sn.tf.draw ->
   
#     # --------------------------------------------------------------
#     sn.tf.hover ->
   
#     # --------------------------------------------------------------
#     sn.tf.keyPressed (key) ->
   
#     # --------------------------------------------------------------
#     sn.tf.keyReleased (key) ->
   
#     # --------------------------------------------------------------
#     sn.tf.click ->
   
#     # --------------------------------------------------------------
#     sn.tf.pointerDown ->

#     # --------------------------------------------------------------
#     sn.tf.pointerEnter ->

#     # --------------------------------------------------------------
#     sn.tf.pointerLeave ->

#     # --------------------------------------------------------------
#     sn.tf.pointerMoved ->

#     # --------------------------------------------------------------
#     sn.tf.pointerOut ->

#     # --------------------------------------------------------------
#     sn.tf.pointerOver ->

#     # --------------------------------------------------------------
#     sn.tf.pointerUp ->
    
#     # --------------------------------------------------------------
#     sn.tf.windowScroll (top) ->
   
#     # --------------------------------------------------------------
#     sn.tf.windowResized (width, height) ->
   
#     # --------------------------------------------------------------
#     sn.tf.fullScreenChange (full) ->