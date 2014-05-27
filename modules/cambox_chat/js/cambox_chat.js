(function ($) {
  Drupal.behaviors.camboxChat = {
    attach: function (context, settings) {
      $('#comment-form', context).once('foo', function () {
        // Ajax Chat Form
        commentFormAjax();
        
        // Ajax Pull
        commentPullAjax();
      });
    }
  };

/**
 * Some global vars.
 */
var pullActive = 0;

/**
 * Ajax Pull Chat.
 */
function commentPullAjax() {
  if ( pullActive == 0 ) {
    pullActive = 1;
    
    // Prepare Vars
    baseUrl = $('#cambox-chat-form').attr('data-pull-url');
    lastCid = $('#comments-raw .comments .comment:last').attr('data-cid');
    baseUrl += lastCid;
    
    // Do Ajax Call
    $.post(baseUrl, function(data){
      if ( data.count > 0 ) {
        $('#comments-raw .comments .comment:last').removeClass('last');
        $('#comments-raw .comments').append(data.comments);
      }
      pullActive = 0;
      commentPullAjax();
    }, 'json');
  }
}

/**
 * Ajax Chat form.
 */
function commentFormAjax() {
  $('#cambox-chat-form').submit(function(){
    url = $(this).attr('action');
    post = {message: $(this).find('.form-textarea').val()};
    $.post(url, post, function(data){
      $('#comments-raw .comments .comment.last').removeClass('last');
      $('#comments-raw .comments').append(data.comment);
      $('#cambox-chat-form .form-textarea').val('');
    }, 'json');
    return false;
  });
}
})(jQuery);