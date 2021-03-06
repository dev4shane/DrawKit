/**
 @author Contributions from the community; see CONTRIBUTORS.md
 @date 2005-2016
 @copyright MPL2; see LICENSE.txt
*/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@protocol DKRouteFinderProgressDelegate;

typedef NS_ENUM(NSInteger, DKRouteAlgorithmType) {
	kDKUseSimulatedAnnealing = 1,
	kDKUseNearestNeighbour = 2
};

typedef NS_ENUM(NSInteger, DKDirection) {
	kDirectionEast = 0,
	kDirectionSouth = 1,
	kDirectionWest = 2,
	kDirectionNorth = 3,
	kDirectionAny = -1
};

/** @brief This object implements an heuristic solution to the travelling salesman problem.

 This object implements an heuristic solution to the travelling salesman problem. The algorithm is based on simulated annealing
 and is due to "Numerical Recipes in C", Chapter 10.

 To use, initialise with an array of <code>NSValue</code>s containing <code>NSPoint</code>s. Then request the <code>shortestRoute</code>. The order of points returned by \c -shortestRoute
 will be the shortest route as determined by the algorithm. The first point object in both input and output arrays is the same - in other words
 the zeroth element of the input array sets the starting point of the path.

 For uses with other object types, the \c -shortestRouteOrder might be more useful. This returns an array of integers which is the order of the
 objects. This can then be used to reorder arbitrary objects.

 Most simply, the \c +sortedArrayOfObjects:byShortestRouteForKey: will deal with any objects as long as they have a KVC-compliant property that
resolves to an \c NSPoint return value, and is given by <code>key</code>. The result is a new array of the same objects sorted according to the TSP solution.
*/
@interface DKRouteFinder : NSObject {
@private
	NSArray<NSValue*>* mInput; // input list of NSPoint values
	DKRouteAlgorithmType mAlgorithm; // which algorithm to use
	NSInteger* mOrder; // final sort order (1-based)
	BOOL mCalculationDone; // flag whether the sort was run
	id<DKRouteFinderProgressDelegate> __weak mProgressDelegate; // a progress delegate, if any
	// for SA
	CGFloat* mX; // for SA, list of input x coordinates
	CGFloat* mY; // for SA, list of input y coordinates
	NSInteger mAnnealingSteps; // for SA, the number of steps in the outer loop
	CGFloat mPathLength; // the path length
	// for NN
	NSMutableArray<NSValue*>* mVisited; // for NN, the list of visited points in visit order
	DKDirection mDirection; // limit search for NN to this direction
}

+ (nullable DKRouteFinder*)routeFinderWithArrayOfPoints:(NSArray<NSValue*>*)arrayOfPoints NS_REFINED_FOR_SWIFT;
+ (nullable DKRouteFinder*)routeFinderWithObjects:(NSArray*)objects withValueForKey:(NSString*)key;
+ (NSArray*)sortedArrayOfObjects:(NSArray*)objects byShortestRouteForKey:(NSString*)key;
@property (class) DKRouteAlgorithmType algorithm;

/** @brief returns the original points reordered into the shortest route.
 */
- (NSArray<NSValue*>*)shortestRoute NS_REFINED_FOR_SWIFT;

/** @brief Returns a list of integers which specifies the shortest route between the original points.
 */
- (NSArray<NSNumber*>*)shortestRouteOrder NS_REFINED_FOR_SWIFT;

/** @brief Sorts \c anArray according to the sort order calculated and returns the sorted copy.
 */
- (nullable NSArray*)sortedArrayFromArray:(NSArray*)anArray;
/** @brief Return the computed path length for the set method. Note this doesn't return a valid result
 during a progress callback, only after the sort has completed.
 */
@property (readonly) CGFloat pathLength;
@property (readonly) DKRouteAlgorithmType algorithm;

@property (weak, nullable) id<DKRouteFinderProgressDelegate> progressDelegate;

@end

#define kDKDefaultAnnealingSteps 100

/** @brief Protocol that an object can implement to be called back as the route finding progresses.
 \c value is in the range 0..1
 */
@protocol DKRouteFinderProgressDelegate <NSObject>

- (void)routeFinder:(DKRouteFinder*)rf progressHasReached:(CGFloat)value;

@end

NS_ASSUME_NONNULL_END
